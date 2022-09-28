//
// Created by 2h 0x on 2022/5/17.
//
#pragma once

#include <memory>
#include <utility>
#include "common.h"
#include "strategy/runner.h"

namespace af
{
    namespace core
    {
        class Trader
        {
        public:
            Trader() :
                    Trader(nullptr)
            {}

            explicit Trader(strategy::RunnerSPtr const &runner) :
                    runner_(runner)
            {}

            virtual void set_runner(strategy::RunnerSPtr const &runner)
            {
                runner_ = runner;
            }

            virtual ~Trader() = default;

            virtual std::string name() = 0;

            virtual std::string version() = 0;

            void set_msg_queue(event::EventQueueSPtr const &msg_queue);

            void send_msg(event::EventType type, void *data);

            /**! \brief 初始化
             * init 要比 on_start 调用得早
             */
            virtual void init() = 0;

            virtual void on_start()
            {}

            virtual void on_stop()
            {}

            virtual bool request_account()
            { return false; }

            virtual bool request_position()
            { return false; }

            virtual type::OrderInputSPtr insert_order(const type::OrderInputSPtr &order_input)
            { return nullptr; }

            virtual bool cancel_order(uint64_t order_id)
            { return false; }

            virtual bool query_all_order()
            { return false; }

        protected:
            event::EventQueueSPtr msg_queue_ = nullptr;
            std::shared_ptr<strategy::Runner> runner_;
        };

        DECLARE_SPTR(Trader);
    }
}
