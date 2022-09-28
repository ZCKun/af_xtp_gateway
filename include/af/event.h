//
// Created by 2h 0x on 2022/5/31.
//

#pragma once
#include "concurrentqueue.h"

namespace af
{
    namespace event
    {
        enum class EventType
        {
            ENTRUST = 0,
            TRANS,
            SYMBOL_INFO,
            TICK,
            LOB,
            ORDER_ERROR,
            ORDER_ACCEPTED,
            ORDER_REJECTED,
            ORDER_CANCELED,
            ORDER_TRADED,
            ORDER_CANCEL_ACCEPTED,
            ORDER_CANCEL_REJECTED,
            QRY_POS,
            QRY_ORDER,
        };

        struct Event
        {
            EventType type;
            void* data;
        };

        using EventQueue = moodycamel::ConcurrentQueue<event::Event>;
        using EventQueueSPtr = std::shared_ptr<EventQueue>;
    }
}
