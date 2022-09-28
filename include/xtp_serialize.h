//
// Created by x2h1z on 22-5-11.
//

#pragma once

#include <string>
#include <fmt/format.h>
#include "api/xtp_api_struct.h"

namespace af
{
    namespace xtp
    {
        inline std::string to_json(XTPOrderInfo const &order_info)
        {
            return fmt::format(
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
        }

        inline std::string to_json(XTPQueryStkPositionRsp const &pos)
        {
            return fmt::format(
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

        inline std::string to_json(XTPTradeReport const &trade_report)
        {
            return fmt::format(
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
        }

        inline std::string to_json(XTPQueryAssetRsp const &asset)
        {
            return fmt::format(
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
        }

        inline std::string to_json(XTPMarketDataStruct const &md)
        {
            return fmt::format(
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

        inline std::string to_json(XTPTickByTickEntrust const &entrust)
        {
            return fmt::format(
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

        inline std::string to_json(XTPTickByTickTrade const &trade)
        {
            return fmt::format(
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

        inline std::string to_json(XTPTickByTickStruct const &tbt)
        {
            return fmt::format(
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
                    to_json(tbt.entrust),
                    to_json(tbt.trade)
            );
        }

        inline std::string to_json(XTPQuoteFullInfo const &info)
        {
            return fmt::format(
                    FMT_STRING("{{"
                               "\"exchange_id\": {},"
                               "\"ticker\": \"{}\","
                               "\"ticker_name\": \"{}\","
                               "\"security_type\": {},"
                               "\"ticker_qualification_class\": {},"
                               "\"is_registration\": {},"
                               "\"is_VIE\": {},"
                               "\"is_noprofit\": {},\""
                               "\"is_weighted_voting_rights\": {},"
                               "\"is_have_price_limit\": {},"
                               "\"upper_limit_price\": {},"
                               "\"lower_limit_price\": {},"
                               "\"pre_close_price\": {},"
                               "\"price_tick\": {},"
                               "\"bid_qty_upper_limit\": {},"
                               "\"bid_qty_lower_limit\": {},"
                               "\"bid_qty_unit\": {},"
                               "\"ask_qty_upper_limit\": {},"
                               "\"ask_qty_lower_limit\": {},"
                               "\"ask_qty_unit\": {},"
                               "\"market_bid_qty_upper_limit\": {},"
                               "\"market_bid_qty_lower_limit\": {},"
                               "\"market_bid_qty_unit\": {},"
                               "\"market_ask_qty_upper_limit\": {},"
                               "\"market_ask_qty_lower_limit\": {},"
                               "\"market_ask_qty_unit\": {}"
                               "}}"),
                    static_cast<int>(info.exchange_id),
                    info.ticker,
                    info.ticker_name,
                    static_cast<int>(info.security_type),
                    static_cast<int>(info.ticker_qualification_class),
                    info.is_registration,
                    info.is_VIE,
                    info.is_noprofit,
                    info.is_weighted_voting_rights,
                    info.is_have_price_limit,
                    info.upper_limit_price,
                    info.lower_limit_price,
                    info.pre_close_price,
                    info.price_tick,
                    info.bid_qty_upper_limit,
                    info.bid_qty_lower_limit,
                    info.bid_qty_unit,
                    info.ask_qty_upper_limit,
                    info.ask_qty_lower_limit,
                    info.ask_qty_unit,
                    info.market_bid_qty_upper_limit,
                    info.market_bid_qty_lower_limit,
                    info.market_bid_qty_unit,
                    info.market_ask_qty_upper_limit,
                    info.market_ask_qty_lower_limit,
                    info.market_ask_qty_unit
            );
        }
    }
}
