//! \file strategy.h
//! Created by x2h1z on 22-5-9.
//! strategy interface
#pragma once

#include "data_type.h"

namespace af
{
    namespace strategy
    {
        FORWARD_DECLARE_PTR(Context);

        /*! Strategy interface */
        class Strategy
        {
        public:
            Strategy() = default;

            Strategy(const Strategy &) = delete;

            Strategy &operator=(const Strategy &) = delete;

            //! \brief 定时器回调
            //!
            //! \param ctx 上下文
            virtual void on_timer(void*)
            {}

            //! \brief 当一切工作准备就绪时调用
            //!
            //! \param ctx 上下文
            virtual void on_start(af::strategy::ContextSPtr ctx)
            {}

            //! \brief stop
            //!
            //! \param ctx 上下文
            virtual void on_stop(af::strategy::ContextSPtr ctx)
            {}

            //! \brief 行情快照消息回调
            //!
            //! \param ctx 上下文
            //! \param quote 快照数据
            virtual void on_tick(af::strategy::ContextSPtr ctx, af::type::TickSPtr const &quote)
            {}

            //! \brief 逐笔委托消息回调
            //!
            //! \param ctx 上下文
            //! \param entrust 逐笔委托数据
            virtual void on_entrust(af::strategy::ContextSPtr ctx, af::type::EntrustSPtr const &entrust)
            {}

            //! \brief 逐笔成交消息回调
            //!
            //! \param ctx 上下文
            //! \param trans 逐笔成交数据
            virtual void on_transaction(af::strategy::ContextSPtr ctx, af::type::TransactionSPtr const &trans)
            {}

            //! \brief 限价订单簿消息回调
            //!
            //! \param ctx 上下文
            //! \param lob 订单簿数据
            virtual void on_lob(af::strategy::ContextSPtr ctx, af::type::OrderBookSPtr const &lob)
            {}

            //! \brief 合约/股票代码基本信息消息回调
            //!
            //! \param ctx 上下文
            //! \param symbol_info 合约/股票代码的一些基本信息(昨收价、股票/合约类型、是否可交易等)
            virtual void
            on_symbol_info(af::strategy::ContextSPtr ctx, af::type::SymbolInfoSPtr const &symbol_info)
            {}

            //! \brief 订单状态更新回调
            //!
            //! \param ctx 上下文
            //! \param order 订单信息
            virtual void on_order(af::strategy::ContextSPtr ctx, af::type::OrderSPtr order)
            {}

            //! \brief [未启用]订单操作错误回调
            //!
            //! \param ctx 上下文
            //! \param error 操作错误信息
            virtual void
            on_order_action_error(af::strategy::ContextSPtr ctx, af::type::OrderActionErrorSPtr error)
            {}

            //! \brief 订单成交消息回调
            //!
            //! \param ctx 上下文
            //! \param trade 成交信息
            virtual void on_trade(af::strategy::ContextSPtr ctx, af::type::TradeSPtr const &trade)
            {}

            //! \brief 订单挂单被拒绝回调
            //!
            //! \param ctx 上下文
            //! \param order 订单信息
            virtual void on_order_rejected(af::strategy::ContextSPtr ctx, af::type::OrderSPtr order)
            {}

            //! \brief 订单撤单被拒绝回调
            //!
            //! \param ctx 上下文
            //! \param order 订单信息
            virtual void on_order_cancel_rejected(af::strategy::ContextSPtr ctx, af::type::OrderSPtr order)
            {}

            //! \brief 订单撤单被接收回调
            //!
            //! \param ctx 上下文
            //! \param order 订单信息
            virtual void on_order_cancel_accept(af::strategy::ContextSPtr ctx, af::type::OrderSPtr order)
            {}

            //! \brief 订单已撤单回调
            //!
            //! \param ctx 上下文
            //! \param order 订单信息
            virtual void on_order_cancelled(af::strategy::ContextSPtr ctx, af::type::OrderSPtr order)
            {}

            //! \brief 订单已撤单失败回调
            //!
            //! \param ctx 上下文
            //! \param order_xtp_id 订单xtp id
            //! \param order_cancel_xtp_id 撤单id
            virtual void
            on_order_cancel_error(af::strategy::ContextSPtr ctx, uint64_t order_xtp_id, uint64_t order_cancel_xtp_id)
            {}
        };

        DECLARE_SPTR(Strategy);
    }
}
