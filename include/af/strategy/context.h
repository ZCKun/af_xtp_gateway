//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <chrono>

#include "core/trader_base.h"
#include "core/market_base.h"
#include "data_type.h"
#include "runner.h"

namespace af
{
    namespace strategy
    {
        class Context
        {
        public:
            Context() = default;

            Context(std::shared_ptr<core::Trader> trader, std::shared_ptr<core::Market> market) :
                    trader_(std::move(trader)),
                    market_(std::move(market))
            {
            }

            void set_trader(std::shared_ptr<core::Trader> const &trader)
            {
                trader_ = trader;
            }

            void set_market(std::shared_ptr<core::Market> const &market)
            {
                market_ = market;
            }

            void on_start();

            //! \brief 设置 runner
            void set_runner(strategy::RunnerSPtr const &runner);

            //! \brief 设置 timer
            void add_timer(uint64_t ms, void* params, Strategy *);

            //! \brief 持仓查询
            bool query_position();

            //! \brief 下单请求
            //!
            //! \param symbol_code 股票/合约代码
            //! \param price 下单价格
            //! \param qty 下单量
            //! \param side 开/平方向
            //! \param exchange 交易所
            //! \return 请求下单是否成功
            type::OrderInputSPtr insert_order(char *symbol_code,
                                              double price,
                                              int64_t qty,
                                              Side side,
                                              type::Exchange exchange);

            //! \brief 请求下单
            //!
            //! \param order 下单参数
            //! \return 请求下单是否成功
            type::OrderInputSPtr insert_order(type::OrderInputSPtr const &order);

            //! \brief 请求撤单
            //!
            //! \param order_id 要撤单的 order xtp id
            //! \return 请求撤单是否成功
            bool cancel_order(uint64_t order_id);

            //! \brief 查询所有股票代码
            //!
            //! \param exchange 交易所
            //! \return 请求是否成功
            bool query_all_symbol_info(type::Exchange exchange);

            //! \brief 订阅限价订单簿
            //!
            //! \param exchange 交易所
            //! \return 请求是否成功
            bool subscribe_all_lob(type::Exchange exchange);

            //! \brief 订阅所有行情
            //!
            //! \param exchange 交易所
            //! \return 请求是否成功
            bool subscribe_all(type::Exchange exchange);

            //! \brief 行情订阅
            //!
            //! \param symbols 股票/合约代码
            //! \return 请求是否成功
            bool subscribe(std::vector<type::Symbol> const &symbols);

            //! \brief 行情订阅
            //!
            //! \param symbols 股票代码
            //! \param exchange 交易所
            //! \return 请求是否成功
            bool subscribe(std::vector<std::string> const &symbols, type::Exchange exchange);

            void set_td_queue(event::EventQueueSPtr const &td_queue);

            //! \brief 查询所有委托
            bool query_all_orders();

        private:
            std::shared_ptr<core::Trader> trader_ = nullptr;
            std::shared_ptr<core::Market> market_ = nullptr;

            std::unordered_map<uint32_t, uint32_t> subscribed_symbols_;
            strategy::RunnerSPtr runner_;

            event::EventQueueSPtr td_queue_ = nullptr;
        };
    }
}
