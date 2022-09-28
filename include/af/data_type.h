//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include <fmt/format.h>
#include <string>
#include <cinttypes>
#include <cstring>
#include <string_view>
#include <functional>

#include "common.h"

#define SYMBOL_LEN 32
#define EXCHANGE_ID_LEN 16
#define ACCOUNT_ID_LEN 32
#define CLIENT_ID_LEN 32
#define ERROR_MSG_LEN 128

#define DATE_LEN 9

using namespace std::literals;

namespace af
{
    namespace type
    {
        FORWARD_DECLARE_PTR(OrderInput);

        FORWARD_DECLARE_PTR(Symbol);

        FORWARD_DECLARE_PTR(Asset);

        FORWARD_DECLARE_PTR(Position);

        FORWARD_DECLARE_PTR(Trade);

        FORWARD_DECLARE_PTR(Order);

        FORWARD_DECLARE_PTR(OrderActionError);

        FORWARD_DECLARE_PTR(Transaction);

        FORWARD_DECLARE_PTR(Entrust);

        FORWARD_DECLARE_PTR(Tick);

        FORWARD_DECLARE_PTR(OrderBook);

        FORWARD_DECLARE_PTR(SymbolInfo);

        /*! 交易所类型 */
        enum Exchange
        {
            SSE = 1,
            SZE = 2
        };

        /*! 股票/合约信息 */
        struct Symbol
        {
            char symbol[SYMBOL_LEN]; /**< 股票/合约代码 */
            Exchange exchange; /**< 交易所类型 */
            SymbolType symbol_type;  /**< 股票/合约类型 */

            int contract_multiplier; /**< 合约乘数 */
            double price_tick; /**< 最小变动价位 */

            std::string symbol_code{};

            Symbol() = default;

            Symbol(std::string const &_symbol, Exchange _exchange) :
                    Symbol(_symbol, _exchange, 0, 0)
            {}

            Symbol(
                    std::string const &_symbol,
                    Exchange _exchange,
                    int _contract_multiplier,
                    double _price_tick,
                    SymbolType _symbol_type = SymbolType::Stock) :
                    exchange(_exchange),
                    symbol_type(_symbol_type),
                    contract_multiplier(_contract_multiplier),
                    price_tick(_price_tick)
            {
                auto bound = SYMBOL_LEN - 1;
                if (_symbol.size() <= bound) {
                    strcpy(symbol, _symbol.c_str());
                } else {
                    strncpy(symbol, _symbol.c_str(), bound);
                }
                symbol[_symbol.size()] = '\0';
                symbol_code = _symbol + (_exchange == Exchange::SZE ? ".SZ" : ".SH");
            }

            Symbol &operator=(Symbol const &s)
            {
                if (this == &s)
                    return *this;

                strcpy(symbol, s.symbol);
                exchange = s.exchange;
                symbol_type = s.symbol_type;
                contract_multiplier = s.contract_multiplier;
                price_tick = s.price_tick;
                symbol_code = s.symbol_code;
                return *this;
            }
        };

        /*! 资金信息 */
        struct Asset
        {
            int64_t update_time; /**< 更新时间 */
            char trading_day[DATE_LEN]; /**< 交易日 */

            uint32_t holder_uid;

            char account_id[ACCOUNT_ID_LEN]; /**< 账户id */

            double avail; /**< 可用资金 */
        };

        /*! 仓位信息 */
        struct Position
        {
            int64_t update_time; /**< 更新时间 */
            char trading_day[DATE_LEN]; /**< 交易日 */

//            char symbol[SYMBOL_LEN]; /**< 股票/合约代码 */
            Symbol symbol;
            Exchange exchange; /**< 交易所类型 */
            char account_id[ACCOUNT_ID_LEN]; /**< 账户id */

            Direction direction; /**< 持仓方向 */

            int64_t volume; /**< 持仓量 */
            int64_t yesterday_volume; /**< 昨仓量 */
            int64_t frozen_total; /**<  */
            int64_t frozen_yesterday;

            double last_price; /**< 最新价 */

            double avg_open_price; /**< 平均开盘价 */
            double position_cost_price; /**< 持仓成本价 */

            double close_price; /**< 收盘价 */
            double pre_close_price; /**< 昨收价 */

            double settlement_price; /**< 结算价 */
            double pre_settlement_price; /**< 昨结算价 */

            double margin;
            double position_pnl;
            double close_pnl;

            double realized_pnl;
            double unrealized_pnl;
        };

        struct Trade
        {
            uint64_t trade_id;
            uint64_t order_id;

            int64_t trade_time;

            Symbol symbol;
            Exchange exchange;
            char account_id[ACCOUNT_ID_LEN];
            char client_id[CLIENT_ID_LEN];

            Side side;

            double price;
            int64_t volume;

            [[nodiscard]] inline bool is_buy() const
            { return side == Side::BUY; }

            [[nodiscard]] inline bool is_sell() const
            { return side == Side::SELL; }
        };

        struct Order
        {
            // FIXME: 委托成交时不会设置
            uint64_t order_id;
            // FIXME: 目前需要用户自己用xtp order id去找下单时设置的order id, 委托成交时会设置
            uint64_t order_xtp_id;

            SymbolType symbol_type;

            int64_t insert_time;
            int64_t update_time;

            char trading_day[DATE_LEN];

            Symbol symbol;
            Exchange exchange;

            char account_id[ACCOUNT_ID_LEN];
            char client_id[CLIENT_ID_LEN];

            double limit_price;
            double frozen_price;

            int64_t qty;
            int64_t qty_traded;
            int64_t qty_left;
            int64_t qty_canceled;

            OrderStatus status = OrderStatus::Unknown;

            int64_t error_id;
            char error_msg[ERROR_MSG_LEN];

            Side side;

            bool is_external;

            Order();

            [[nodiscard]] std::string get_trading_day() const
            { return trading_day; }

            void set_trading_day(std::string const &_trading_day)
            {
                strncpy(this->trading_day, _trading_day.c_str(), DATE_LEN);
            }

            [[nodiscard]] std::string get_side() const
            { return side == Side::BUY ? "BUY" : "SELL"; }

            [[nodiscard]] bool is_cancel() const
            { return status == OrderStatus::Canceled || status == OrderStatus::PartialFilledNotActive; }

            [[nodiscard]] bool is_pending() const
            { return status == OrderStatus::Pending; }
        };

        struct OrderActionError
        {
            uint64_t order_id;
            uint64_t order_action_id;
            int32_t error_id;
            uint64_t xtp_order_id;
            char error_msg[ERROR_MSG_LEN];

            [[nodiscard]] std::string get_error_msg() const
            { return error_msg; }

            void set_error_msg(std::string const &_error_msg)
            {
                strncpy(error_msg, _error_msg.c_str(), ERROR_MSG_LEN);
            }

        }; //__attribute__((packed));

        struct Transaction
        {
            char trading_day[DATE_LEN];

            int64_t data_time;
            int64_t recv_time;

            Symbol symbol;
            // char exchange_id[EXCHANGE_ID_LEN];
            Exchange exchange;

            double price;
            int64_t volume;

            int64_t bid_no;
            int64_t ask_no;

            ExecType exec_type;
            BsFlag bs_flag;

            int64_t main_seq;
            int64_t seq;

            Transaction();

            [[nodiscard]] std::string get_exec_type() const
            {
                return exec_type == ExecType::Unknown
                       ? "UNKNOWN" : exec_type == ExecType::Cancel
                                     ? "CANCEL" : "TRADE";
            }

            [[nodiscard]] std::string get_bs_flag() const
            {
                return (bs_flag == BsFlag::Unknown) ? "UNKNOWN" : (bs_flag == BsFlag::Buy) ? "BUY" : "SELL";
            }

            [[nodiscard]] inline bool is_cancel() const
            { return exec_type == ExecType::Cancel; }

            [[nodiscard]] inline bool is_trade() const noexcept
            { return exec_type == ExecType::Trade; }

            [[nodiscard]] inline bool trade_buy() const noexcept
            { return ask_no > bid_no; }

            [[nodiscard]] inline bool trade_sell() const noexcept
            { return bid_no > ask_no; }

        };

        struct Entrust
        {
            char trading_day[DATE_LEN];

            int64_t data_time;
            int64_t recv_time;

            Symbol symbol;
            Exchange exchange;

            double price;
            int64_t volume;
            Side side;
            PriceType price_type;
            ExecType exec_type;

            int64_t main_seq;
            int64_t order_id;

            [[nodiscard]] inline bool is_sz() const noexcept
            { return exchange == Exchange::SZE; }

            [[nodiscard]] inline bool is_sh() const noexcept
            { return exchange == Exchange::SSE; }

            [[nodiscard]] inline bool is_buy() const noexcept
            { return side == Side::BUY; }

            [[nodiscard]] inline bool is_sell() const noexcept
            { return side == Side::SELL; }

            [[nodiscard]] inline bool is_cancel() const noexcept
            { return is_sh() && exec_type == ExecType::Cancel; }

            [[nodiscard]] std::string get_price_type() const
            {
                switch (price_type) {
                    case PriceType::LIMIT:
                        return "LIMIT";
                        break;
                    case PriceType::ANY:
                        return "ANY";
                        break;
                    case PriceType::FakBest5:
                        return "FAK_BEST5";
                        break;
                    case PriceType::ForwardBest:
                        return "FORWARD_BEST";
                        break;
                    case PriceType::ReverseBest:
                        return "REVERSE_BEST";
                        break;
                    case PriceType::Fak:
                        return "FAK";
                        break;
                    case PriceType::Fok:
                        return "FOK";
                        break;
                    default:
                    case PriceType::UnKnown:
                        return "UNKNOWN";
                        break;
                }
            }
        };

        struct Tick
        {
            /** 交易日 **/
            char trading_day[DATE_LEN];
            /** 数据生成时间 **/
            int64_t data_time;
            /** 数据接收时间 **/
            int64_t recv_time;

//            SymbolType symbol_type;

            /** 股票代码 **/
            Symbol symbol;
            /** 交易所 **/
//            Exchange exchange;

            /** 昨日收盘价 **/
            double pre_close_price;
            /** 昨日结算价 **/
            double pre_settlement_price;

            /** 最新价 **/
            double last_price;
            /** 数量 **/
            int64_t volume;
            /** 成交金额 **/
            double turnover;

            /** 昨日持仓量 **/
            int64_t pre_open_interest;
            /** 持仓量 **/
            int64_t open_interest;

            /** 开盘价 **/
            double open_price;
            /** 最高价 **/
            double high_price;
            /** 最低价 **/
            double low_price;

            /** 涨停价 **/
            double upper_limit_price;
            /** 跌停价 **/
            double lower_limit_price;

            /** 收盘价 **/
            double close_price;
            double settlement_price;

            /** 申买价 **/
            double bid_price[10];
            /** 申卖价 **/
            double ask_price[10];
            /** 申买量 **/
            int64_t bid_volume[10];
            /** 申卖量 **/
            int64_t ask_volume[10];
        };

        struct OrderBook
        {
            char trading_day[DATE_LEN];
            int64_t data_time;
            int64_t recv_time;

            Symbol symbol;
            Exchange exchange;

            int64_t trades_count;

            double last_price;
            int64_t volume;
            double turnover;

            double bid[10];
            double ask[10];
            int64_t bid_qty[10];
            int64_t ask_qty[10];
        };

        struct OrderInput
        {
            uint64_t order_id;
            uint64_t order_xtp_id;
            char symbol[SYMBOL_LEN];
            Exchange exchange;
            char account_id[ACCOUNT_ID_LEN];

            //> SymbolType
            double limit_price;
            int64_t qty;
            Side side;
            VolumeCondition volume_condition;
            TimeCondition time_condition;

            [[nodiscard]] std::string get_symbol() const
            { return symbol; }

            void set_symbol(std::string const &_symbol)
            { strncpy(this->symbol, _symbol.c_str(), SYMBOL_LEN); }

            std::string get_account_id()
            { return account_id; }

            void set_account_id(std::string const &_account_id)
            {
                strncpy(this->account_id, _account_id.c_str(), ACCOUNT_ID_LEN);
            }

            [[nodiscard]] std::string get_side() const
            { return side == Side::BUY ? "BUY" : "SELL"; }

            [[nodiscard]] inline std::string to_string() const
            {
#if 1
                return fmt::format(
                        FMT_STRING("{{"
                                   R"("order_id":{},)"
                                   R"("symbol":"{}",)"
                                   R"("exchange_id":"{}",)"
                                   R"("account_id":"{}",)"
                                   R"("limit_price":{},)"
                                   R"("qty":{},)"
                                   R"("side":{})"
                                   "}}"sv),
                        order_id,
                        symbol,
                        exchange,
                        account_id,
                        limit_price,
                        qty,
                        get_side()
                );
#else
                return "";
#endif
            }

        }; // __attribute__((packed));

        struct SymbolInfo
        {
            char trading_day[DATE_LEN];

            Symbol symbol;
            Exchange exchange;

            double pre_close_price;
            double price_tick;

            bool is_have_price_limit;
            double upper_limit_price;
            double lower_limit_price;

            int security_type;

            int64_t bid_qty_unit;
            int64_t ask_qty_unit;

            [[nodiscard]] inline bool is_sz() const
            { return exchange == Exchange::SZE; }

            [[nodiscard]] inline bool is_sh() const
            { return !is_sz(); }
        };
    }
}

template<>
struct fmt::formatter<af::type::SymbolInfo>
{
    template<typename Context>
    constexpr auto parse(Context &context)
    {
        return std::begin(context);
    }

    template<typename Context>
    auto format(const af::type::SymbolInfo &info, Context &context)
    {
        return fmt::format_to(
                context.out(),
                FMT_STRING("{{"
                           R"("trading_day": "{}",,)"
                           R"("symbol": "{}",,)"
                           R"("exchange": "{}",,)"
                           R"("pre_close_price": {},)"
                           R"("price_tick": {},)"
                           R"("bid_qty_unit": {},)"
                           R"("ask_qty_unit": {},)"
                           R"("is_hava_price_limit": {},)"
                           R"("upper_limit_price": {},)"
                           R"("lower_limit_price": {})"
                           "}}"sv),
                info.trading_day,
                info.symbol,
                info.exchange,
                info.pre_close_price,
                info.price_tick,
                info.bid_qty_unit,
                info.ask_qty_unit,
                info.is_have_price_limit,
                info.upper_limit_price,
                info.lower_limit_price
        );
    }
};

template<>
struct fmt::formatter<af::type::Tick>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::Tick const &quote, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trading_day": "{}",)"
                           R"("data_time": {},)"
                           R"("recv_time": {},)"
                           R"("symbol": "{}",)"
                           R"("pre_close_price": {},)"
                           R"("pre_settlement_price": {},)"
                           R"("last_price": {},)"
                           R"("qty": {},)"
                           R"("turnover": {},)"
                           R"("pre_open_interest": {},)"
                           R"("open_interest": {},)"
                           R"("open_price": {},)"
                           R"("high_price": {},)"
                           R"("low_price": {},)"
                           R"("upper_limit_price": {},)"
                           R"("lower_limit_price": {},)"
                           R"("close_price": {},)"
                           R"("settlement_price": {},)"
                           R"("bid1": {},)"
                           R"("ask1": {},)"
                           R"("bid_qty1": {},)"
                           R"("ask_qty1": {})"
                           "}}"sv),
                quote.trading_day,
                quote.data_time,
                quote.recv_time,
                quote.symbol,
                quote.pre_close_price,
                quote.pre_settlement_price,
                quote.last_price,
                quote.volume,
                quote.turnover,
                quote.pre_open_interest,
                quote.open_interest,
                quote.open_price,
                quote.high_price,
                quote.low_price,
                quote.upper_limit_price,
                quote.lower_limit_price,
                quote.close_price,
                quote.settlement_price,
                quote.bid_price[0],
                quote.ask_price[0],
                quote.bid_volume[0],
                quote.ask_volume[0]
        );
    }
};

template<>
struct fmt::formatter<af::type::Trade>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    { return ctx.begin(); }

    template<typename FormatContext>
    auto format(af::type::Trade const &trade, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trade_id": {},)"
                           R"("order_id": {},)"
                           R"("trade_time": {},)"
                           R"("symbol": "{}",)"
                           R"("exchanges": "{}",)"
                           R"("account_id": "{}",)"
                           R"("client_id": "{}",)"
                           R"("side": {},)"
                           R"("price": {},)"
                           R"("qty": {})"
                           "}}"sv),
                trade.trade_id,
                trade.order_id,
                trade.trade_time,
                trade.symbol,
                trade.exchange,
                trade.account_id,
                trade.client_id,
                trade.side == af::Side::BUY ? "BUY" : "SELL",
                trade.price,
                trade.volume
        );
    }
};

template<>
struct fmt::formatter<af::type::Order>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    { return ctx.begin(); }

    template<typename FormatContext>
    auto format(af::type::Order const &order, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trading_day":"{}",)"
                           R"("order_id":{},)"
                           R"("order_xtp_id:":{},)"
                           R"("client_id:":{},)"
                           R"("symbol":"{}",)"
                           R"("exchange":"{}",)"
                           R"("account_id":"{}",)"
                           R"("limit_price":{},)"
                           R"("insert_time":{},)"
                           R"("update_time":{},)"
                           R"("qty":{},)"
                           R"("qty_left":{},)"
                           R"("qty_traded":{},)"
                           R"("qty_canceled":{},)"
                           R"("side":"{}",)"
                           R"("status":{},)"
                           R"("error_id":{},)"
                           R"("error_msg":"{}")"
                           "}}"sv),
                order.trading_day,
                order.order_id,
                order.order_xtp_id,
                order.client_id,
                order.symbol,
                order.exchange,
                order.account_id,
                order.limit_price,
                order.insert_time,
                order.update_time,
                order.qty,
                order.qty_left,
                order.qty_traded,
                order.qty_canceled,
                order.get_side(),
                static_cast<int>(order.status),
                order.error_id,
                order.error_msg
        );
    }
};

template<>
struct fmt::formatter<af::type::OrderActionError>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::OrderActionError const order_error, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("order_id": {},)"
                           R"("order_action_id": {},)"
                           R"("error_id": {},)"
                           R"("xtp_order_id": {},)"
                           R"("error_msg": "{}")"
                           "}}"sv),
                order_error.order_id,
                order_error.order_action_id,
                order_error.error_id,
                order_error.xtp_order_id,
                order_error.error_msg
        );
    }
};

template<>
struct fmt::formatter<af::type::Entrust>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    { return ctx.begin(); }

    template<typename FormatContext>
    auto format(af::type::Entrust const &entrust, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trading_day":"{}",)"
                           R"("data_time": {},)"
                           R"("recv_time": {},)"
                           R"("symbol":"{}",)"
                           R"("exchange":"{}",)"
                           R"("price": {},)"
                           R"("qty":{},)"
                           R"("side":"{}",)"
                           R"("price_type":"{}",)"
                           R"("main_seq": {},)"
                           R"("seq": {})"
                           "}}"sv),
                entrust.trading_day,
                entrust.data_time,
                entrust.recv_time,
                entrust.symbol,
                entrust.exchange,
                entrust.price,
                entrust.volume,
                entrust.side == af::Side::BUY ? "BUY" : "SELL",
                entrust.get_price_type(),
                entrust.main_seq,
                entrust.order_id
        );
    }
};

template<>
struct fmt::formatter<af::type::Transaction>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    { return ctx.begin(); }

    template<typename FormatContext>
    auto format(af::type::Transaction const &trans, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trading_day": "{}",)"
                           R"("data_time": {},)"
                           R"("recv_time": {},)"
                           R"("symbol": "{}",)"
                           R"("exchange": "{}",)"
                           R"("price": {},)"
                           R"("qty": {},)"
                           R"("bid_no": {},)"
                           R"("ask_no": {},)"
                           R"("exec_type": "{}",)"
                           R"("bs_flag": "{}",)"
                           R"("main_seq": {},)"
                           R"("seq": {})"
                           "}}"sv),
                trans.trading_day,
                trans.data_time,
                trans.recv_time,
                trans.symbol,
                trans.exchange,
                trans.price,
                trans.volume,
                trans.bid_no,
                trans.ask_no,
                trans.get_exec_type(),
                trans.get_bs_flag(),
                trans.main_seq,
                trans.seq
        );
    }
};

template<>
struct fmt::formatter<af::type::OrderBook>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::OrderBook const &lob, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("trading_day": "{}",)"
                           R"("data_time": {},)"
                           R"("recv_time": {},)"
                           R"("symbol": "{}",)"
                           R"("exchange": "{}",)"
                           R"("trades_count": {},)"
                           R"("last_price": {},)"
                           R"("qty": {},)"
                           R"("turnover": {},)"
                           R"("bid1": {},)"
                           R"("ask1": {},)"
                           R"("bid_qty1": {},)"
                           R"("ask_qty1": {})"
                           "}}"sv),
                lob.trading_day,
                lob.data_time,
                lob.recv_time,
                lob.symbol,
                lob.exchange,
                lob.trades_count,
                lob.last_price,
                lob.volume,
                lob.turnover,
                lob.bid[0],
                lob.ask[0],
                lob.bid_qty[0],
                lob.ask_qty[0]
        );
    }
};

template<>
struct fmt::formatter<af::type::Symbol>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::Symbol const &symbol, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           R"("symbol": "{}",)"
                           R"("exchange": "{}",)"
                           R"("symbol_type": {},)"
                           R"("contract_multiplier": {},)"
                           R"("price_tick": {},)"
                           R"("symbol_code": "{}")"
                           "}}"sv),
                symbol.symbol,
                symbol.exchange,
                static_cast<int8_t>(symbol.symbol_type),
                symbol.contract_multiplier,
                symbol.price_tick,
                symbol.symbol_code
        );
    }
};

namespace std
{
    template<>
    struct hash<af::type::Symbol>
    {
        using result_type = std::size_t;
        using argument_type = af::type::Symbol;

        result_type operator()(argument_type const &symbol) const
        {
            return std::hash<std::string>{}(symbol.symbol) ^
                   (std::hash<int32_t>{}(symbol.exchange) << 1);
        }
    };
}

//static_assert(sizeof(af::type::Trade) == 200);
//static_assert(sizeof(af::type::Order) == 416);
//static_assert(sizeof(af::type::Symbol) == 88);
//static_assert(sizeof(af::type::OrderBook) == 480);
//static_assert(sizeof(af::type::Entrust) == 168);
//static_assert(sizeof(af::type::Transaction) == 184);
