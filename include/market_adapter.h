//
// Created by x2h1z on 22-5-10.
//
#pragma once

#include <vector>
#include <af/core/market_base.h>
#include <af/event.h>

#include "api/xtp_quote_api.h"
#include "xtp_serialize.h"
#include "concurrentqueue.h"
#include "config.h"

namespace af
{
    namespace strategy
    {
        class Runner;
    }

    namespace xtp
    {
        class Market : public core::Market, public XTP::API::QuoteSpi
        {
        public:
            Market() = default;

            Market(Config const &config, const std::shared_ptr<af::strategy::Runner> &runner);

            Market(std::string const &config_path, strategy::RunnerSPtr const &runner);

            void set_msg_queue(std::shared_ptr<moodycamel::ConcurrentQueue<event::Event>> const &msg_queue);

            void set_queue(const event::EventQueueSPtr &queue);

            void init() override;

            void on_start() override;

            std::string name() override;

            std::string version() override;

            bool subscribe(std::vector<type::Symbol> const &symbols) override;

            bool subscribe_all(type::Exchange exchange) override;

            bool subscribe_all_orderbook(type::Exchange exchange) override;

            bool query_all_tickers_full_info(type::Exchange exchange) override;

            void OnDisconnected(int reason) override;

            void OnQueryAllTickersFullInfo(XTPQFI *ticker_info, XTPRI *error_info, bool is_last) override;

            void OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) override;

            void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) override;

            void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last) override;

            void OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) override;

            void OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) override;

            void
            OnDepthMarketData(XTPMD *market_data, int64_t *bid1_qty, int32_t bid1_count, int32_t max_bid1_count,
                              int64_t *ask1_qty, int32_t ask1_count, int32_t max_ask1_count) override;

            void OnTickByTick(XTPTBT *tbt_data) override;

            void OnOrderBook(XTPOB *order_book) override;

            void OnError(XTPRI *error_info) override;

        private:
            Config config_;
            std::string account_id_;
            event::EventQueueSPtr msg_queue_;
            event::EventQueueSPtr md_queue_;

            XTP::API::QuoteApi *api_;
            std::string trading_day_;

            virtual bool subscribe(std::vector<std::string> const &symbols, type::Exchange exchange_id);

            template<typename T>
            void send_msg(event::EventType type, T *data);

            void on_entrust(XTPTBT *tbt_data, int64_t now_ns);

            void on_trans(XTPTBT *tbt_data, int64_t now_ns);

            void login();
        };
    }
}