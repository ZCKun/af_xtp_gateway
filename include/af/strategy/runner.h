//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include "strategy.h"
#include "concurrentqueue.h"
#include "event.h"

#include <flat_hash_map.h>
#include <map>
#include <vector>
#include <chrono>

namespace af
{
    namespace strategy
    {
        class Runner
        {
            friend Context;
        public:
            Runner();

            bool run();

            void stop();

            void set_md_queue(const event::EventQueueSPtr &queue);

            void set_td_queue(const event::EventQueueSPtr &queue);

            void set_msg_queue(event::EventQueueSPtr const &msg_queue);

            void set_context(ContextSPtr const &ctx);

            void add_strategy(StrategySPtr const &strategy);

            void on_symbol_info(type::SymbolInfoSPtr const &info, bool is_last);

            void on_lob(type::OrderBookSPtr const &lob) const;

            void on_tick(const type::TickSPtr &quote) const;

            void on_order(type::OrderSPtr const &order);

            void on_order_rejected(type::OrderSPtr const &order);

            void on_order_cancel_accept(const type::OrderSPtr &order);

            void on_order_cancel_rejected(const type::OrderSPtr &order);

            void on_order_cancel_error(uint64_t order_xtp_id, uint64_t order_cancel_xtp_id);

            void on_order_cancelled(type::OrderSPtr &order);

            void on_trade(type::TradeSPtr const &trade);

            void on_order_action_error(type::OrderActionErrorSPtr const &action_error);

            void on_entrust(type::EntrustSPtr const &entrust) const;

            void on_transaction(type::TransactionSPtr const &transaction) const;

            template<typename T>
            std::shared_ptr<T> quote_type()
            {

            }

            template<typename RecordType>
            using RecordCallback = std::function<void(ska::flat_hash_map<int64_t, RecordType> const &)>;

            int trans_record(type::Symbol const &symbol,
                             int64_t duration_ns,
                             RecordCallback<type::TransactionSPtr> const &callback);

        protected:
            std::vector<StrategySPtr> strategies_;

            void on_start();

            void on_exit();

            void add_timer(uint64_t ms, void*, Strategy *);

        private:
            ContextSPtr context_;
            event::EventQueueSPtr msg_queue_;
            event::EventQueueSPtr md_queue_;
            event::EventQueueSPtr td_queue_;
            bool running_ = true;

            int record_id_ = 0;
            std::map<int64_t, int, std::greater<>> record_tasks_;
            ska::flat_hash_map<int, type::Symbol> record_symbols_;
            ska::flat_hash_map<int, RecordCallback<type::TransactionSPtr>> record_callbacks_;
            ska::flat_hash_map<std::string, ska::flat_hash_map<int64_t, type::TransactionSPtr>> trans_cache_;
            ska::flat_hash_map<std::string, ska::flat_hash_map<int64_t, type::EntrustSPtr>> entrust_cache_;

            void event_listen();

            void on_event(const event::Event &event);

            void td_event_listen();

            void quote_record_thread();
        };

        DECLARE_SPTR(Runner);
    }
}