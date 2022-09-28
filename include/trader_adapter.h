//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <af/core/trader_base.h>
#include <af/event.h>

#include "api/xtp_trader_api.h"
#include "config.h"
#include "concurrentqueue.h"

namespace af
{
    namespace strategy
    {
        class Runner;
    }
    namespace xtp
    {
        class Trader : public core::Trader, public XTP::API::TraderSpi
        {
        public:
            Trader() = default;

            Trader(Config const& , strategy::RunnerSPtr const &runner);
//
            Trader(std::string const &config_path, strategy::RunnerSPtr const &runner);

            [[nodiscard]] const std::string &get_account_id() const
            {
                return account_id_;
            }

            [[nodiscard]] const std::string &get_trading_day() const
            {
                return trading_day_;
            }

            std::string name() override;

            std::string version() override;

            void init() override;

            void set_queue(event::EventQueueSPtr const &td_queue);

            template<typename T>
            void send_td_msg(event::EventType type, T *data);

            void on_start() override;

            bool query_all_order() override;

            bool request_account() override;

            bool request_position() override;

            type::OrderInputSPtr insert_order(type::OrderInputSPtr const &order_input) override;

            bool cancel_order(uint64_t order_xtp_id) override;

            void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) override;

            void OnDisconnected(uint64_t session_id, int reason) override;

            void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) override;

            void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) override;

            void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last,
                              uint64_t session_id) override;

            void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last,
                                 uint64_t session_id) override;

            void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last,
                              uint64_t session_id) override;

        private:
            Config config_;
            XTP::API::TraderApi *trader_api_;
            uint64_t session_id_;
            int32_t request_id_;
            std::string account_id_;
            std::string trading_day_;
            event::EventQueueSPtr td_queue_ = nullptr;

            std::vector<strategy::StrategySPtr> strategies_;

            void login();

            void on_order_info(XTPOrderInfo *, XTPRI *, bool);
        };
    }
}
