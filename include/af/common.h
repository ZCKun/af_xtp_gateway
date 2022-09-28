//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include <cinttypes>
#include <memory>
#include <magic_enum.hpp>

#define DECLARE_SPTR(X) typedef std::shared_ptr<X> X##SPtr;
#define FORWARD_DECLARE_PTR(X) class X; DECLARE_SPTR(X)

namespace af
{
    enum class BsFlag: int8_t
    {
        Unknown,
        Buy,
        Sell
    };

    enum class ExecType: int8_t
    {
        Unknown,
        Cancel,
        Trade,
        Order
    };

    enum class PriceType: int8_t
    {
        LIMIT,                  //限价,证券通用
        ANY,                    //市价，证券通用，对于股票上海为最优五档剩余撤销，深圳为即时成交剩余撤销
        FakBest5,               //上海深圳最优五档即时成交剩余撤销，不需要报价
        ForwardBest,            //深圳本方方最优价格申报, 不需要报价
        ReverseBest,            //上海最优五档即时成交剩余转限价, 深圳对手方最优价格申报，不需要报价
        Fak,                    //深圳即时成交剩余撤销，不需要报价
        Fok,                    //深圳市价全额成交或者撤销，不需要报价
        UnKnown
    };

    enum class Side : int8_t
    {
        BUY = 1,
        SELL
    };

    enum class OrderStatus : int8_t
    {
        Unknown,
        Submitted,
        Pending,
        Accepted,
        Canceled,
        Rejected,
        Error,
        Filled,
        PartialFilledNotActive,
        PartialFilledActive,
        CancelAccepted,
        CancelRejected
    };

    enum class SymbolType: int8_t
    {
        Unknown = 0,       //未知
        Stock,         //普通股票
        Bond,          //债券
        StockOption,   //股票期权
        Fund,          //基金
        TechStock,     //科创板股票
        Index,         //指数
        Repo           //回购
    };

    enum class Direction : int8_t
    {
        Long,
        Short
    };
}
