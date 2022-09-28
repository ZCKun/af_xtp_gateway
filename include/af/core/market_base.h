//
// Created by 2h 0x on 2022/5/17.
//

#pragma once

#include <utility>
#include <vector>
#include "data_type.h"
#include "strategy/runner.h"

namespace af
{
    namespace core
    {
        class Market
        {
        public:
            Market() :
                    runner_(nullptr)
            {}

            Market(std::shared_ptr<strategy::Runner> runner) :
                    runner_(std::move(runner))
            {}

            virtual ~Market() = default;

            virtual void init() = 0;

            virtual std::string version() = 0;

            virtual std::string name() = 0;

            virtual void set_runner(std::shared_ptr<strategy::Runner> const &runner)
            {
                runner_ = runner;
            }

            virtual void on_start()
            {}

            virtual void on_stop()
            {}

            virtual bool subscribe(const std::vector<type::Symbol> &symbols)
            { return false; }

            virtual bool subscribe_all(type::Exchange exchange)
            { return false; }

            virtual bool subscribe_all_orderbook(type::Exchange exchange)
            { return false; }

            virtual bool query_all_tickers_full_info(type::Exchange exchange)
            { return false; }

        protected:
            std::shared_ptr<strategy::Runner> runner_;
        };

        DECLARE_SPTR(Market);
    }
}
