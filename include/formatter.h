//
// Created by x2h1z on 22-5-9.
//

#ifndef AF_FORMATTER_H
#define AF_FORMATTER_H

#include <fmt/format.h>
#include "data_type.h"
#include "api/xtp_api_struct.h"

#if 0
template<>
struct fmt::formatter<XTPOrderInfo>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPOrderInfo const &order_info, FormatContext &ctx) -> decltype(ctx.out())
    {
#if 1
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"order_xtp_id\":{},"
                           "\"order_client_id\":{},"
                           "\"order_cancel_client_id\":{},"
                           "\"order_cancel_xtp_id\":{},"
                           "\"ticker\":\"{}\","
                           "\"market\":{},"
                           "\"price\":{},"
                           "\"quantity\":{},"
                           "\"price_type\":{},"
                           "\"side\":{},"
                           "\"business_type\":{},"
                           "\"qty_traded\":{},"
                           "\"qty_left\":{},"
                           "\"insert_time\":{},"
                           "\"update_time\":{},"
                           "\"cancel_time\":{},"
                           "\"trade_amount\":{},"
                           "\"order_local_id\":{},"
                           "\"order_status\":{},"
                           "\"order_submit_status\":{},"
                           "\"order_type\":{}"
                           "}}"),
                order_info.order_xtp_id,
                order_info.order_client_id,
                order_info.order_cancel_client_id,
                order_info.order_cancel_xtp_id,
                order_info.ticker,
                order_info.market,
                order_info.price,
                order_info.quantity,
                order_info.price_type,
                order_info.side,
                order_info.business_type,
                order_info.qty_traded,
                order_info.qty_left,
                order_info.insert_time,
                order_info.update_time,
                order_info.cancel_time,
                order_info.trade_amount,
                order_info.order_local_id,
                order_info.order_status,
                order_info.order_submit_status,
                order_info.order_type
        );
#else
        return format_to(ctx.out(), "");
#endif
    }
};

template<>
struct fmt::formatter<XTPTradeReport>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPTradeReport const &trade_report, FormatContext &ctx) -> decltype(ctx.out())
    {
#if 1
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"order_xtp_id\":{},"
                           "\"order_client_id\":{},"
                           "\"ticker\":\"{}\","
                           "\"market\":{},"
                           "\"local_order_id\":{},"
                           "\"exec_id\":{},"
                           "\"price\":{},"
                           "\"quantity\":{},"
                           "\"trade_time\":{},"
                           "\"trade_amount\":{},"
                           "\"report_index\":{},"
                           "\"order_exch_id\":{},"
                           "\"trade_type\":{},"
                           "\"business_type\":{},"
                           "\"side\":{},"
                           "\"position_effect\":{}"
                           "}}"),
                trade_report.order_xtp_id,
                trade_report.order_client_id,
                trade_report.ticker,
                trade_report.market,
                trade_report.local_order_id,
                trade_report.exec_id,
                trade_report.price,
                trade_report.quantity,
                trade_report.trade_time,
                trade_report.trade_amount,
                trade_report.report_index,
                trade_report.order_exch_id,
                trade_report.trade_type,
                trade_report.business_type,
                trade_report.side,
                trade_report.position_effect
        );
#else
        return format_to(ctx.out(), "");
#endif
    }
};


template<>
struct fmt::formatter<XTPQueryAssetRsp>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPQueryAssetRsp const &asset, FormatContext &ctx) -> decltype(ctx.out())
    {
#if 0
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"total_asset\":{},"
                           "\"buying_power\":{},"
                           "\"security_asset\":{},"
                           "\"fund_buy_amount\":{},"
                           "\"fund_buy_fee\":{},"
                           "\"fund_sell_amount\":{},"
                           "\"fund_sell_fee\":{},"
                           "\"withholding_amount\":{},"
                           "\"account_type\":{},"
                           "\"frozen_margin\":{},"
                           "\"frozen_exec_cash\":{}."
                           "\"frozen_exec_fee\":{},"
                           "\"pay_later\":{},"
                           "\"peradva_pay\":{},"
                           "\"orig_banlance\":{},"
                           "\"banlance\":{},"
                           "\"deposit_withdraw\":{},"
                           "\"trade_netting\":{},"
                           "\"captial_asset\":{},"
                           "\"force_freeze_amount\":{},"
                           "\"preferred_amount\":{},"
                           "\"repay_stock_aval_banlance\":{},"
                           "\"fund_order_data_charges\":{},"
                           "\"fund_cancel_data_charges\":{},"
                           "\"exchange_cur_risk_degree\":{},"
                           "\"company_cur_risk_degree\":{}"
                           "}}"),
                asset.total_asset,
                asset.buying_power,
                asset.security_asset,
                asset.fund_buy_amount,
                asset.fund_buy_fee,
                asset.fund_sell_amount,
                asset.fund_sell_fee,
                asset.withholding_amount,
                asset.account_type,
                asset.frozen_margin,
                asset.frozen_exec_cash,
                asset.frozen_exec_fee,
                asset.pay_later,
                asset.preadva_pay,
                asset.orig_banlance,
                asset.banlance,
                asset.deposit_withdraw,
                asset.trade_netting,
                asset.captial_asset,
                asset.force_freeze_amount,
                asset.preferred_amount,
                asset.repay_stock_aval_banlance,
                asset.fund_order_data_charges,
                asset.fund_cancel_data_charges,
                asset.exchange_cur_risk_degree,
                asset.company_cur_risk_degree
        );
#else
        return fmt::format_to(ctx.out(), "");
#endif
    }

};

template<>
struct fmt::formatter<XTPQueryStkPositionRsp>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    { return ctx.begin(); }

    template<typename FormatContext>
    auto format(XTPQueryStkPositionRsp const &pos, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"ticker\":\"{}\","
                           "\"ticker_name\":\"{}\","
                           "\"market\":{},"
                           "\"total_qty\":{},"
                           "\"sellable_qty\":{},"
                           "\"avg_price\":{},"
                           "\"unrealized_pnl\":{},"
                           "\"yesterday_position\":{},"
                           "\"purchase_redeemable_qty\":{},"
                           "\"position_direction\":{},"
                           "\"position_security_type\":{},"
                           "\"executable_option\":{},"
                           "\"lockable_position\":{},"
                           "\"executable_underlying\":{},"
                           "\"locked_position\":{},"
                           "\"usable_locked_position\":{},"
                           "\"profit_price\":{},"
                           "\"buy_cost\":{},"
                           "\"profit_cost\":{}"
                           "}}"),
                pos.ticker,
                pos.ticker_name,
                pos.market,
                pos.total_qty,
                pos.sellable_qty,
                pos.avg_price,
                pos.unrealized_pnl,
                pos.yesterday_position,
                pos.purchase_redeemable_qty,
                static_cast<int>(pos.position_direction),
                static_cast<int>(pos.position_security_type),
                pos.executable_option,
                pos.lockable_position,
                pos.executable_underlying,
                pos.locked_position,
                pos.usable_locked_position,
                pos.profit_price,
                pos.buy_cost,
                pos.profit_cost
        );
    }
};

template<>
struct fmt::formatter<XTPMarketDataStruct>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPMarketDataStruct const &md, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"exchange_id\": {},"
                           "\"ticker\":\"{}\","
                           "\"last_price\":{},"
                           "\"pre_close_price\":{},"
                           "\"open_price\":{},"
                           "\"high_price\":{},"
                           "\"low_price\":{},"
                           "\"close_price\":{},"
                           "\"pre_total_long_position\":{},"
                           "\"total_long_position\":{},"
                           "\"pre_settl_price\":{},"
                           "\"settl_price\":{},"
                           "\"upper_limit_price\":{},"
                           "\"lower_limit_price\":{},"
                           "\"pre_delta\":{},"
                           "\"curr_delta\":{},"
                           "\"data_time\":{},"
                           "\"qty\":{},"
                           "\"turnover\":{},"
                           "\"avg_price\":{},"
                           "\"bid\":{},"
                           "\"ask\":{},"
                           "\"bid_qty\":{},"
                           "\"ask_qty\":{},"
                           "\"trades_count\":{},"
                           "\"ticker_status\":\"{}\","
                           "\"data_type\":{}"
                           "}}"),
                static_cast<int>(md.exchange_id),
                md.ticker,
                md.last_price,
                md.pre_close_price,
                md.open_price,
                md.high_price,
                md.low_price,
                md.close_price,
                md.pre_total_long_positon,
                md.total_long_positon,
                md.pre_settl_price,
                md.settl_price,
                md.upper_limit_price,
                md.lower_limit_price,
                md.pre_delta,
                md.curr_delta,
                md.data_time,
                md.qty,
                md.turnover,
                md.avg_price,
                "",
                "",
                "",
                "",
                md.trades_count,
                md.ticker_status,
                static_cast<int>(md.data_type)
        );
    }
};

template<>
struct fmt::formatter<XTPTickByTickEntrust>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPTickByTickEntrust const &entrust, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"channel_no\": {},"
                           "\"seq\": {},"
                           "\"price\": {},"
                           "\"qty\": {},"
                           "\"side\": \"{}\","
                           "\"ord_type\": \"{}\""
                           "}}"),
                entrust.channel_no,
                entrust.seq,
                entrust.price,
                entrust.qty,
                entrust.side,
                entrust.ord_type
        );
    }
};

template<>
struct fmt::formatter<XTPTickByTickTrade>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPTickByTickTrade const &trade, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"channel_no\": {},"
                           "\"seq\": {},"
                           "\"price\": {},"
                           "\"qty\": {},"
                           "\"money\": {},"
                           "\"bid_no\": {},"
                           "\"ask_no\": {},"
                           "\"trade_flag\": \"{}\""
                           "}}"),
                trade.channel_no,
                trade.seq,
                trade.price,
                trade.qty,
                trade.money,
                trade.bid_no,
                trade.ask_no,
                trade.trade_flag
        );
    }
};

template<>
struct fmt::formatter<XTPTickByTickStruct>
{
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(XTPTickByTickStruct const &tbt, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"exchange_id\": {},"
                           "\"ticker\": \"{}\","
                           "\"seq\": {},"
                           "\"data_time\": {},"
                           "\"type\": {},"
                           "\"entrust\": {},"
                           "\"trade\": {}"
                           "}}"),
                static_cast<int>(tbt.exchange_id),
                tbt.ticker,
                tbt.seq,
                tbt.data_time,
                static_cast<int>(tbt.type),
                tbt.entrust,
                tbt.trade
        );
    }
};

#endif

//template<>
//struct fmt::formatter<af::Quote>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    {
//        return ctx.begin();
//    }
//
//    template<typename FormatContext>
//    auto format(af::Quote const &quote, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"trading_day\": \"{}\","
//                           "\"data_time\": {},"
//                           "\"recv_time\": {},"
//                           "\"symbol\": \"{}\","
//                           "\"exchange_id\": \"{}\","
//                           "\"pre_close_price\": {},"
//                           "\"pre_settlement_price\": {},"
//                           "\"last_price\": {},"
//                           "\"qty\": {},"
//                           "\"turnover\": {},"
//                           "\"pre_open_interest\": {},"
//                           "\"open_interest\": {},"
//                           "\"open_price\": {},"
//                           "\"high_price\": {},"
//                           "\"low_price\": {},"
//                           "\"upper_limit_price\": {},"
//                           "\"lower_limit_price\": {},"
//                           "\"close_price\": {},"
//                           "\"settlement_price\": {},"
//                           "\"bid1\": {},"
//                           "\"ask1\": {},"
//                           "\"bid_qty1\": {},"
//                           "\"ask_qty1\": {}"
//                           "}}"),
//                quote.trading_day,
//                quote.data_time,
//                quote.recv_time,
//                quote.symbol,
//                quote.exchange_id,
//                quote.pre_close_price,
//                quote.pre_settlement_price,
//                quote.last_price,
//                quote.qty,
//                quote.turnover,
//                quote.pre_open_interest,
//                quote.open_interest,
//                quote.open_price,
//                quote.high_price,
//                quote.low_price,
//                quote.upper_limit_price,
//                quote.lower_limit_price,
//                quote.close_price,
//                quote.settlement_price,
//                quote.bid_price[0],
//                quote.ask_price[0],
//                quote.bid_volume[0],
//                quote.ask_volume[0]
//        );
//    }
//
//};

//template<>
//struct fmt::formatter<af::Trade>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    { return ctx.begin(); }
//
//    template<typename FormatContext>
//    auto format(af::Trade const &trade, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return fmt::format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"trade_id\": {},"
//                           "\"order_id\": {},"
//                           "\"trade_time\": {},"
//                           "\"symbol\": \"{}\","
//                           "\"exchange_id\": \"{}\","
//                           "\"account_id\": \"{}\","
//                           "\"client_id\": \"{}\","
//                           "\"side\": {},"
//                           "\"price\": {},"
//                           "\"qty\": {}"
//                           "}}"),
//                trade.trade_id,
//                trade.order_id,
//                trade.trade_time,
//                trade.symbol,
//                trade.exchange_id,
//                trade.account_id,
//                trade.client_id,
//                trade.side == af::Side::BUY ? "BUY" : "SELL",
//                trade.price,
//                trade.qty
//        );
//    }
//};
//

//template<>
//struct fmt::formatter<af::Order>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    { return ctx.begin(); }
//
//    template<typename FormatContext>
//    auto format(af::Order const &order, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return fmt::format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"order_id\":{},"
//                           "\"symbol\":\"{}\","
//                           "\"exchange_id\":\"{}\","
//                           "\"account_id\":\"{}\","
//                           "\"limit_price\":{},"
//                           "\"qty\":{},"
//                           "\"side\":\"{}\","
//                           "\"error_id\":{},"
//                           "\"error_msg\":\"{}\""
//                           "}}"),
//                order.order_id,
//                order.symbol,
//                order.exchange_id,
//                order.account_id,
//                order.limit_price,
//                order.qty,
//                order.get_side(),
//                order.error_id,
//                order.error_msg
//        );
//    }
//};

//template<>
//struct fmt::formatter<af::OrderActionError>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    {
//        return ctx.begin();
//    }
//
//    template<typename FormatContext>
//    auto format(af::OrderActionError const order_error, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"order_id\": {},"
//                           "\"order_action_id\": {},"
//                           "\"error_id\": {},"
//                           "\"xtp_order_id\": {},"
//                           "\"error_msg\": \"{}\""
//                           "}}"),
//                order_error.order_id,
//                order_error.order_action_id,
//                order_error.error_id,
//                order_error.xtp_order_id,
//                order_error.error_msg
//        );
//    }
//};

//template<>
//struct fmt::formatter<af::Entrust>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    { return ctx.begin(); }
//
//    template<typename FormatContext>
//    auto format(af::Entrust const &entrust, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return fmt::format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"trading_day\":\"{}\","
//                           "\"data_time\": {},"
//                           "\"recv_time\": {},"
//                           "\"symbol\":\"{}\","
//                           "\"exchange_id\":\"{}\","
//                           "\"price\": {},"
//                           "\"qty\":{},"
//                           "\"side\":\"{}\","
//                           "\"price_type\":\"{}\","
//                           "\"main_seq\": {},"
//                           "\"seq\": {}"
//                           "}}"),
//                entrust.trading_day,
//                entrust.data_time,
//                entrust.recv_time,
//                entrust.symbol,
//                entrust.exchange_id,
//                entrust.price,
//                entrust.qty,
//                entrust.side == af::Side::BUY ? "BUY" : "SELL",
//                entrust.get_price_type(),
//                entrust.main_seq,
//                entrust.seq
//        );
//    }
//};
//
//template<>
//struct fmt::formatter<af::Transaction>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    { return ctx.begin(); }
//
//    template<typename FormatContext>
//    auto format(af::Transaction const &trans, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return fmt::format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"trading_day\": \"{}\","
//                           "\"data_time\": {},"
//                           "\"recv_time\": {},"
//                           "\"symbol\": \"{}\","
//                           "\"exchange_id\": \"{}\","
//                           "\"price\": {},"
//                           "\"qty\": {},"
//                           "\"bid_no\": {},"
//                           "\"ask_no\": {},"
//                           "\"exec_type\": \"{}\","
//                           "\"bs_flag\": \"{}\","
//                           "\"main_seq\": {},"
//                           "\"seq\": {}"
//                           "}}"),
//                trans.trading_day,
//                trans.data_time,
//                trans.recv_time,
//                trans.symbol,
//                trans.exchange_id,
//                trans.price,
//                trans.qty,
//                trans.bid_no,
//                trans.ask_no,
//                trans.get_exec_type(),
//                trans.get_bs_flag(),
//                trans.main_seq,
//                trans.seq
//        );
//    }
//};
//
//template<>
//struct fmt::formatter<af::OrderBook>
//{
//    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
//    {
//        return ctx.begin();
//    }
//
//    template<typename FormatContext>
//    auto format(af::OrderBook const &lob, FormatContext &ctx) -> decltype(ctx.out())
//    {
//        return format_to(
//                ctx.out(),
//                FMT_STRING("{{"
//                           "\"trading_day\": \"{}\","
//                           "\"data_time\": {},"
//                           "\"recv_time\": {},"
//                           "\"symbol\": \"{}\","
//                           "\"exchange_id\": \"{}\","
//                           "\"trades_count\": {},"
//                           "\"last_price\": {},"
//                           "\"qty\": {},"
//                           "\"turnover\": {},"
//                           "\"bid1\": {},"
//                           "\"ask1\": {},"
//                           "\"bid_qty1\": {},"
//                           "\"ask_qty1\": {}"
//                           "}}"),
//                lob.trading_day,
//                lob.data_time,
//                lob.recv_time,
//                lob.symbol,
//                lob.exchange_id,
//                lob.trades_count,
//                lob.last_price,
//                lob.qty,
//                lob.turnover,
//                lob.bid[0],
//                lob.ask[0],
//                lob.bid_qty[0],
//                lob.ask_qty[0]
//        );
//    }
//};
//
#if 0
template<>
struct fmt::formatter<af::type::OrderInput> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::OrderInput const &input, FormatContext &ctx) -> decltype(ctx.out()) {
        auto const& vc = input.volume_condition;
        std::string volume_condition = (vc == af::VolumeCondition::All ? "ALL" : vc == af::VolumeCondition::ANY ? "ANY" : "MIN");
        auto const& tc = input.time_condition;
        std::string time_condition = (tc == af::TimeCondition::GFD ? "GFD" : tc == af::TimeCondition::GTC ? "GTC" : "IOC");
        auto const& s = input.side;
        std::string side = (s == af::Side::BUY ? "BUY": "SELL");

        return format_to(
                ctx.out(),
                FMT_STRING("{{"
                           "\"order_id\":{},"
                           "\"symbol\":\"{}\","
                           "\"exchange_id\":\"{}\","
                           "\"account_id\":\"{}\","
                           "\"limit_price\":{},"
                           "\"qty\":{},"
                           "\"side\":{},"
                           "\"volume_condition\":{},"
                           "\"time_condition\":{},"
                           "}}"),
                input.order_id,
                input.symbol,
                input.exchange_id,
                input.account_id,
                input.limit_price,
                input.qty,
                side,
                volume_condition,
                time_condition
        );
    }
};

template<>
struct fmt::formatter<af::type::OrderInput> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(af::type::OrderInput const &input, FormatContext &ctx) -> decltype(ctx.out()) {
        return format_to(
                ctx.out(),
        );

    }

};
#endif

#endif //AF_FORMATTER_H
