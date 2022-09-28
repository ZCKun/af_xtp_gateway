//
// Created by x2h1z on 22-5-10.
//
#include <af/log.h>
#include <af/utils/common.h>
#include "strategy/runner.h"
#include "market_adapter.h"

namespace af
{
    namespace xtp
    {
        Market::Market(Config const& config, const std::shared_ptr<af::strategy::Runner> &runner) :
                core::Market(runner),
                config_(config),
                account_id_(config_.username),
                msg_queue_(nullptr),
                md_queue_(nullptr),
                api_(nullptr)
        {}

        Market::Market(const std::string &config_path, const strategy::RunnerSPtr &runner) :
                Market(Config(config_path), runner)
        {}

        void Market::init()
        {
            LOG_PRINT_INFO("Connecting XTP MD for {} at {}://{}:{}", config_.username, config_.protocol, config_.md_ip,
                           config_.md_port);
            api_ = XTP::API::QuoteApi::CreateQuoteApi(config_.client_id, "./log/xtp/");
            if (config_.protocol == "udp") {
                api_->SetUDPBufferSize(config_.buffer_size);
            }
            api_->RegisterSpi(this);
            login();
            trading_day_ = api_->GetTradingDay();
        }

        void xtp::Market::on_start()
        {
        }

        inline void Market::login()
        {
            int status = api_->Login(
                    config_.md_ip.c_str(),
                    config_.md_port,
                    config_.username.c_str(),
                    config_.password.c_str(),
                    config_.protocol == "udp" ? XTP_PROTOCOL_UDP : XTP_PROTOCOL_TCP
            );
            if (status == 0) {
                LOG_PRINT_INFO("login success! (user_id) {}", config_.username);
            } else {
                XTPRI *error_info = api_->GetApiLastError();
                LOG_PRINT_ERR("login failed. (ErrorID) {}, (ErrorMsg) {}", error_info->error_id, error_info->error_msg);
            }
        }

        bool Market::subscribe(const std::vector<type::Symbol> &symbols)
        {
            LOG_PRINT_INFO("size: {}", symbols.size());
            bool ret = true;

            std::vector<std::string> sse_tickers;
            std::vector<std::string> sze_tickers;

            for (auto const &item: symbols) {
                std::string ticker = item.symbol;
                if (item.exchange == type::Exchange::SSE) {
                    sse_tickers.push_back(ticker);
                } else if (item.exchange == type::Exchange::SZE) {
                    sze_tickers.push_back(ticker);
                }
            }
            if (!sse_tickers.empty()) {
                ret = (ret && subscribe(sse_tickers, type::Exchange::SSE));
            }
            if (!sze_tickers.empty()) {
                ret = (ret && subscribe(sze_tickers, type::Exchange::SZE));
            }

            return ret;
        }

        bool Market::subscribe(const std::vector<std::string> &symbols, type::Exchange exchange_id)
        {
            int symbol_count = static_cast<int>(symbols.size());
            std::vector<char *> ss;
            ss.reserve(symbol_count);
            for (auto const &item: symbols) {
                ss.push_back((char *) &item[0]);
            }

            XTP_EXCHANGE_TYPE exchange = XTP_EXCHANGE_UNKNOWN;
            if (exchange_id == type::Exchange::SSE) {
                exchange = XTP_EXCHANGE_SH;
            } else if (exchange_id == type::Exchange::SZE) {
                exchange = XTP_EXCHANGE_SZ;
            }

            int status = api_->SubscribeMarketData(ss.data(), symbol_count, exchange);
            if (status != 0) {
                LOG_PRINT_ERR("failed to subscribe.");
            } else {
                LOG_PRINT_INFO("subscribe market data request success, (count={}) (exchange_id={})", symbol_count,
                               exchange_id);
            }

            int l2_status = api_->SubscribeTickByTick(ss.data(), symbol_count, exchange);
            if (l2_status != 0) {
                LOG_PRINT_ERR("failed to subscribe tick by tick.");
            } else {
                LOG_PRINT_INFO("subscribe tick by tick request success, (count={}) (exchange_id={})", symbol_count,
                               exchange_id);
            }

            return status == 0 && l2_status == 0;
        }

        bool Market::query_all_tickers_full_info(type::Exchange exchange)
        {
            XTP_EXCHANGE_TYPE exch = XTP_EXCHANGE_UNKNOWN;
            if (exchange == type::Exchange::SZE) {
                exch = XTP_EXCHANGE_SZ;
            } else if (exchange == type::Exchange::SSE) {
                exch = XTP_EXCHANGE_SH;
            }

            if (exch != XTP_EXCHANGE_UNKNOWN) {
                auto status = api_->QueryAllTickersFullInfo(exch);
                LOG_PRINT_INFO("query all tickers full info, (status={}) (exchange={})", status, exchange);
                return status == 0;
            } else {
                auto sh_status = api_->QueryAllTickersFullInfo(XTP_EXCHANGE_SH);
                LOG_PRINT_INFO("query all tickers full info, (status={}) (exchange={})", sh_status, "SSE");
                auto sz_status = api_->QueryAllTickersFullInfo(XTP_EXCHANGE_SZ);
                LOG_PRINT_INFO("query all tickers full info, (status={}) (exchange={})", sz_status, "SZE");
                return sh_status == 0 && sz_status == 0;
            }
        }

        bool Market::subscribe_all(type::Exchange exchange)
        {
            XTP_EXCHANGE_TYPE exch = XTP_EXCHANGE_UNKNOWN;
            if (exchange == type::Exchange::SZE) {
                exch = XTP_EXCHANGE_SZ;
            } else if (exchange == type::Exchange::SSE) {
                exch = XTP_EXCHANGE_SH;
            }

            auto md_s = api_->SubscribeAllMarketData(exch);
            auto tbt_s = api_->SubscribeAllTickByTick(exch);
            auto ob_s = api_->SubscribeAllOrderBook(exch);
            LOG_PRINT_INFO("subscribe all, (md_status={}) (tbt_status={}) (ob_status={}) (exchange={})",
                           md_s, tbt_s, ob_s, exchange);
            return md_s && tbt_s;
        }

        bool Market::subscribe_all_orderbook(type::Exchange exchange)
        {
            XTP_EXCHANGE_TYPE exch = XTP_EXCHANGE_UNKNOWN;
            if (exchange == type::Exchange::SZE) {
                exch = XTP_EXCHANGE_SZ;
            } else if (exchange == type::Exchange::SSE) {
                exch = XTP_EXCHANGE_SH;
            }

            auto ob_s = api_->SubscribeAllOrderBook(exch);
            LOG_PRINT_INFO("subscribe all orderbook, (ob_status={}) (exchange={})", ob_s, exchange);
            return ob_s == 0;
        }

        void Market::OnDisconnected(int reason)
        {
            LOG_PRINT_ERR("quote disconnected, (reason={}), try to login again...", reason);
            auto s = api_->Logout();
            if (s == 0) {
                LOG_PRINT_INFO("logout");
                login();
            } else {
                auto err = api_->GetApiLastError();
                LOG_PRINT_ERR("logout fail, (error_id={}) (error_msg={})", err->error_id, err->error_msg);
            }
        }

        void Market::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("subscribe market data failed, (error_id={}) (error_msg={}) (is_last={})",
                              error_info->error_id, error_info->error_msg,
                              is_last);
            } else {
                LOG_PRINT_INFO("subscribe market data success, (ticker={}) (exchange_id={}) (is_last={})",
                               ticker->ticker, ticker->exchange_id,
                               is_last);
            }
        }

        void Market::OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("subscribe TBT failed, (error_id={}) (error_msg={}) (is_last={})", error_info->error_id,
                              error_info->error_msg,
                              is_last);
            } else {
                LOG_PRINT_INFO("subscribe TBT success, (ticker={}) (exchange_id={}) (is_last={})", ticker->ticker,
                               ticker->exchange_id,
                               is_last);
            }
        }

        void Market::OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("subscribe all orderbook failed, (error_id={}) (error_msg={})", error_info->error_id,
                              error_info->error_msg);
            } else {
                LOG_PRINT_INFO("subscribe all orderbook success, exchange: {}", exchange_id);
            }
        }

        void Market::OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("subscribe all TBT failed, (error_id={}) (error_msg={})", error_info->error_id,
                              error_info->error_msg);
            } else {
                LOG_PRINT_INFO("subscribe all tick by tick success, exchange: {}", exchange_id);
            }
        }

        void Market::OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("subscribe all market data failed, (error_id={}) (error_msg={})", error_info->error_id,
                              error_info->error_msg);
            } else {
                LOG_PRINT_INFO("subscribe all market data success, exchange: {}", exchange_id);
            }
        }

        void
        Market::OnDepthMarketData(XTPMD *market_data, int64_t *bid1_qty, int32_t bid1_count, int32_t max_bid1_count,
                                     int64_t *ask1_qty, int32_t ask1_count, int32_t max_ask1_count)
        {
            int64_t now_ns = util::now_ns();
//            LOG_PRINT_DBG("{}", to_json(*market_data));

            auto tick = util::new_obj<type::Tick>();
            strcpy(tick->trading_day, trading_day_.c_str());
            auto exchange = market_data->exchange_id == XTP_EXCHANGE_SH
                             ? type::Exchange::SSE : type::Exchange::SZE;
            tick->symbol = {market_data->ticker, exchange};
            tick->pre_close_price = market_data->pre_close_price;
            tick->pre_settlement_price = market_data->pre_settl_price;
            tick->last_price = market_data->last_price;
            tick->volume = market_data->qty;
            tick->turnover = market_data->turnover;
            tick->pre_open_interest = market_data->pre_total_long_positon;
            tick->open_interest = market_data->total_long_positon;
            tick->open_price = market_data->open_price;
            tick->high_price = market_data->high_price;
            tick->low_price = market_data->low_price;
            tick->upper_limit_price = market_data->upper_limit_price;
            tick->lower_limit_price = market_data->lower_limit_price;
            tick->close_price = market_data->close_price;
            tick->settlement_price = market_data->settl_price;
            tick->data_time = market_data->data_time;
            tick->recv_time = now_ns;

            switch (market_data->data_type_v2) {
                case XTP_MARKETDATA_V2_INDEX:
                    tick->symbol.symbol_type = SymbolType::Index;
                    break;
                case XTP_MARKETDATA_V2_OPTION:
                    tick->symbol.symbol_type = SymbolType::StockOption;
                    break;
                case XTP_MARKETDATA_V2_ACTUAL:
                    tick->symbol.symbol_type = SymbolType::Stock;
                    break;
                case XTP_MARKETDATA_V2_BOND:
                    tick->symbol.symbol_type = SymbolType::Bond;
                    break;
            }

            for (int i = 0; i < 10; i++) {
                tick->bid_price[i] = market_data->bid[i];
                tick->ask_price[i] = market_data->ask[i];
                tick->bid_volume[i] = market_data->bid_qty[i];
                tick->ask_volume[i] = market_data->ask_qty[i];
            }

            send_msg(event::EventType::TICK, tick);
        }

        void Market::OnTickByTick(XTPTBT *tbt_data)
        {
            int64_t now_ns = util::now_ns();

            if (tbt_data->type == XTP_TBT_ENTRUST) {
                on_entrust(tbt_data, now_ns);
            } else if (tbt_data->type == XTP_TBT_TRADE) {
                on_trans(tbt_data, now_ns);
            }
        }

        void Market::OnOrderBook(XTPOB *order_book)
        {
            int64_t now_ns = util::now_ns();

            auto lob = util::new_obj<type::OrderBook>();
            strcpy(lob->trading_day, trading_day_.c_str());
            lob->exchange = order_book->exchange_id == XTP_EXCHANGE_SH
                            ? type::Exchange::SSE : type::Exchange::SZE;
            lob->symbol = {order_book->ticker, lob->exchange};
            lob->data_time = order_book->data_time;
            lob->recv_time = now_ns;
            lob->trades_count = order_book->trades_count;
            lob->last_price = order_book->last_price;
            lob->volume = order_book->qty;
            lob->turnover = order_book->turnover;

            for (int i = 0; i < 10; i++) {
                lob->bid[i] = order_book->bid[i];
                lob->ask[i] = order_book->ask[i];
                lob->bid_qty[i] = order_book->bid_qty[i];
                lob->ask_qty[i] = order_book->ask_qty[i];
            }

            send_msg(event::EventType::LOB, lob);
        }

        void Market::OnQueryAllTickersFullInfo(XTPQFI *ticker_info, XTPRI *error_info, bool is_last)
        {
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("query all tickers full info failed, (error_id={}) (error_msg={}) (is_last={})",
                              error_info->error_id, error_info->error_id,
                              is_last);
            } else {
                auto symbol_info = util::new_obj<type::SymbolInfo>();
                strcpy(symbol_info->trading_day, trading_day_.c_str());
                symbol_info->exchange = (ticker_info->exchange_id == XTP_EXCHANGE_SH)
                                        ? type::Exchange::SSE : type::Exchange::SZE;
                symbol_info->symbol = {ticker_info->ticker, symbol_info->exchange};
                symbol_info->pre_close_price = ticker_info->pre_close_price;
                symbol_info->price_tick = ticker_info->price_tick;
                symbol_info->bid_qty_unit = ticker_info->bid_qty_unit;
                symbol_info->ask_qty_unit = ticker_info->ask_qty_unit;
                symbol_info->is_have_price_limit = ticker_info->is_have_price_limit;
                symbol_info->upper_limit_price = ticker_info->upper_limit_price;
                symbol_info->lower_limit_price = ticker_info->lower_limit_price;
                symbol_info->security_type = ticker_info->security_type;

                send_msg(event::EventType::SYMBOL_INFO, symbol_info);

                LOG_PRINT_INFO(
                        "query all ticker full info success, (symbol={}) (security_type={}) (ticker_qualification_class={}) (exchange_id={})",
                        ticker_info->ticker, ticker_info->security_type,
                        ticker_info->ticker_qualification_class, ticker_info->exchange_id);
                LOG_PRINT_DBG("{}", *symbol_info);
            }
        }

        void Market::OnError(XTPRI *error_info)
        {
            LOG_PRINT_ERR("(error_id) {} (error_msg) {}", error_info->error_id, error_info->error_msg);
        }

        void Market::set_msg_queue(std::shared_ptr<moodycamel::ConcurrentQueue<event::Event>> const &msg_queue)
        {
            assert(msg_queue != nullptr);
            if (msg_queue == nullptr) {
                LOG_PRINT_ERR("The msg_queue is null.");
                return;
            }
            msg_queue_ = msg_queue;
        }

        void Market::set_queue(event::EventQueueSPtr const &queue)
        {
            assert(queue != nullptr);
            md_queue_ = queue;
        }

        template<typename T>
        inline void Market::send_msg(event::EventType type, T *data)
        {
            assert(msg_queue_ != nullptr);
            event::Event event{};
            event.type = type;
            event.data = reinterpret_cast<void *>(data);
            msg_queue_->enqueue(event);
        }

        inline void Market::on_entrust(XTPTBT *tbt_data, int64_t now_ns)
        {
//            LOG_PRINT_DBG("(recv){} (delay){} {}", now_ns, now_ns - tbt_data->data_time, to_json(*tbt_data));

            auto entrust = util::new_obj<type::Entrust>();
            strncpy(entrust->trading_day, trading_day_.c_str(), DATE_LEN);
            entrust->exchange = tbt_data->exchange_id == XTP_EXCHANGE_SH
                                ? type::Exchange::SSE : type::Exchange::SZE;
            entrust->symbol = {tbt_data->ticker, entrust->exchange};
            entrust->main_seq = tbt_data->seq;
            entrust->data_time = tbt_data->data_time;
            entrust->recv_time = now_ns;
            entrust->price = tbt_data->entrust.price;
            entrust->volume = tbt_data->entrust.qty;
            entrust->order_id = (entrust->exchange == type::Exchange::SZE)
                                ? tbt_data->entrust.seq : tbt_data->entrust.order_no;

            if (tbt_data->entrust.side == '1' || tbt_data->entrust.side == 'B') {
                entrust->side = Side::BUY;
            } else if (tbt_data->entrust.side == '2' || tbt_data->entrust.side == 'S') {
                entrust->side = Side::SELL;
            }

            if (entrust->exchange == type::Exchange::SZE) {
                if (tbt_data->entrust.ord_type == '1') {
                    entrust->price_type = PriceType::ANY;
                } else if (tbt_data->entrust.ord_type == '2') {
                    entrust->price_type = PriceType::LIMIT;
                } else if (tbt_data->entrust.ord_type == 'U') {
                    entrust->price_type = PriceType::ForwardBest;
                }
            } else {
                if (tbt_data->entrust.ord_type == 'D') {
                    entrust->exec_type = ExecType::Cancel;
                } else if (tbt_data->entrust.ord_type == 'A') {
                    entrust->exec_type = ExecType::Order;
                } else {
                    entrust->exec_type = ExecType::Unknown;
                }
            }

            send_msg(event::EventType::ENTRUST, entrust);
        }

        inline void Market::on_trans(XTPTBT *tbt_data, int64_t now_ns)
        {
//            LOG_PRINT_DBG("(recv){} (delay){} {}", now_ns, now_ns - tbt_data->data_time, to_json(*tbt_data));

            auto trans = util::new_obj<type::Transaction>();
            strncpy(trans->trading_day, trading_day_.c_str(), DATE_LEN);
            trans->exchange = tbt_data->exchange_id == XTP_EXCHANGE_SH
                              ? type::Exchange::SSE : type::Exchange::SZE;
            trans->symbol = {tbt_data->ticker, trans->exchange};
            trans->main_seq = tbt_data->seq;
            trans->seq = tbt_data->trade.seq;
            trans->data_time = tbt_data->data_time;
            trans->recv_time = now_ns;
            trans->price = tbt_data->trade.price;
            trans->volume = tbt_data->trade.qty;
            trans->bid_no = tbt_data->trade.bid_no;
            trans->ask_no = tbt_data->trade.ask_no;

            switch (tbt_data->trade.trade_flag) {
                case 'B': {
                    trans->bs_flag = BsFlag::Buy;
                    break;
                }
                case 'S': {
                    trans->bs_flag = BsFlag::Sell;
                    break;
                }
                case 'N': {
                    trans->bs_flag = BsFlag::Unknown;
                    break;
                }
                case '4': {
                    trans->exec_type = ExecType::Cancel;
                    break;
                }
                case 'F': {
                    trans->exec_type = ExecType::Trade;
                    break;
                }
                default:
                    break;
            }

            send_msg(event::EventType::TRANS, trans);
        }

        std::string Market::name()
        {
            return "xtp";
        }

        std::string Market::version()
        {
            return "2.2.33.5";
        }
    }
}
