//
// Created by x2h1z on 22-5-9.
//

#include "log.h"
#include "trader_adapter.h"
#include "formatter.h"
//#include "utils/common.h"
#include <af/utils/common.h>
#include <af/strategy/runner.h>

namespace af
{
    namespace xtp
    {
        inline void from_xtp(const XTP_ORDER_STATUS_TYPE &xtp_order_status, OrderStatus &status)
        {
            if (xtp_order_status == XTP_ORDER_STATUS_INIT || xtp_order_status == XTP_ORDER_STATUS_NOTRADEQUEUEING) {
                status = OrderStatus::Pending;
            } else if (xtp_order_status == XTP_ORDER_STATUS_ALLTRADED) {
                status = OrderStatus::Filled;
            } else if (xtp_order_status == XTP_ORDER_STATUS_CANCELED) {
                status = OrderStatus::Canceled;
            } else if (xtp_order_status == XTP_ORDER_STATUS_PARTTRADEDQUEUEING) {
                status = OrderStatus::PartialFilledActive;
            } else if (xtp_order_status == XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING) {
                status = OrderStatus::PartialFilledNotActive;
            } else if (xtp_order_status == XTP_ORDER_STATUS_REJECTED) {
                status = OrderStatus::Error;
            } else {
                status = OrderStatus::Unknown;
            }
        }

        Trader::Trader(af::xtp::Config const& config, strategy::RunnerSPtr const &runner) :
                core::Trader(runner),
                config_(config),
                trader_api_(nullptr),
                session_id_(-1),
                request_id_(0),
                account_id_(config_.username)
        {}

        Trader::Trader(const std::string &config_path, const strategy::RunnerSPtr &runner) :
                Trader(Config(config_path), runner)
        {}

        void Trader::init()
        {
            LOG_PRINT_INFO("Connecting XTP TD for {} at tcp://{}:{}", config_.username, config_.td_ip,
                           config_.td_port);
            trader_api_ = XTP::API::TraderApi::CreateTraderApi(config_.client_id, "./log/xtp/",
                                                               XTP_LOG_LEVEL_INFO);
            trader_api_->RegisterSpi(this);
            trader_api_->SubscribePublicTopic(XTP_TERT_QUICK);
            trader_api_->SetSoftwareVersion("1.0.0");
            trader_api_->SetSoftwareKey(config_.software_key.c_str());
            trading_day_ = trader_api_->GetTradingDay();
            login();
        }

        void Trader::on_start()
        {
        }

        inline void Trader::login()
        {
            session_id_ = trader_api_->Login(
                    config_.td_ip.c_str(),
                    config_.td_port,
                    config_.username.c_str(),
                    config_.password.c_str(),
                    XTP_PROTOCOL_TCP
            );
            if (session_id_ > 0) {
                LOG_PRINT_INFO("login success");
                LOG_PRINT_DBG("request_account: {}", request_account() ? "success" : "fail");
            } else {
                XTPRI *err = trader_api_->GetApiLastError();
                LOG_PRINT_ERR("login failed, error_id:{}, error_msg:{}", err->error_id, err->error_msg);
            }
        }

        bool Trader::request_position()
        {
            assert(trader_api_ != nullptr);
            int rtn = trader_api_->QueryPosition(nullptr, session_id_, ++request_id_);
            if (rtn != 0) {
                auto err = trader_api_->GetApiLastError();
                LOG_PRINT_ERR("request query position found error, error_id={}, error_msg={}",
                              err->error_id, err->error_msg);
                return false;
            }
            return true;
        }

        bool Trader::request_account()
        {
            assert(trader_api_ != nullptr);
            int rtn = trader_api_->QueryAsset(session_id_, ++request_id_);
            if (rtn != 0) {
                auto err = trader_api_->GetApiLastError();
                LOG_PRINT_ERR("request query asset found error, error_id={}, error_msg={}",
                              err->error_id, err->error_msg);
                return false;
            }
            return true;
        }

        inline void Trader::on_order_info(XTPOrderInfo *order_info, XTPRI *error_info, bool external)
        {
            auto order = util::new_obj<type::Order>();
            strcpy(order->trading_day, trading_day_.c_str());
            strcpy(order->account_id, get_account_id().c_str());
            order->exchange = order_info->market == XTP_MKT_SH_A
                              ? type::Exchange::SSE : type::Exchange::SZE;
            order->symbol = {order_info->ticker, order->exchange};
            order->qty = order_info->quantity;
            order->qty_traded = order_info->qty_traded;
            order->qty_left = order_info->qty_left;
            order->limit_price = order_info->price;
            order->side = order_info->side == XTP_SIDE_BUY ? Side::BUY : Side::SELL;
            from_xtp(order_info->order_status, order->status);
            order->order_xtp_id = order_info->order_xtp_id;
            order->insert_time = order_info->insert_time;
            order->is_external = external;

            if (order_info->business_type == XTP_BUSINESS_TYPE_CASH) {
                order->symbol_type = SymbolType::Stock;
            }

            if (order_info->update_time > 0) {
                order->update_time = order_info->update_time;
            }
            if (error_info != nullptr) {
                order->error_id = error_info->error_id;
                strncpy(order->error_msg, error_info->error_msg, ERROR_MSG_LEN);
            }

            switch (order_info->order_status) {
                case XTP_ORDER_STATUS_INIT:
                case XTP_ORDER_STATUS_NOTRADEQUEUEING: {
                    order->status = OrderStatus::Pending;
                    send_msg(event::EventType::ORDER_ACCEPTED, order);
                    break;
                }
                case XTP_ORDER_STATUS_ALLTRADED:
                    order->status = OrderStatus::Filled;
                    send_msg(event::EventType::ORDER_TRADED, order);
                    break;
                case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
                    order->status = OrderStatus::PartialFilledActive;
                    send_msg(event::EventType::ORDER_TRADED, order);
                    break;
                case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
                    order->status = OrderStatus::PartialFilledNotActive;
                    order->qty_canceled = order_info->qty_left;
                    send_msg(event::EventType::ORDER_CANCELED, order);
                    break;
                case XTP_ORDER_STATUS_CANCELED:
                    order->status = OrderStatus::Canceled;
                    order->qty_canceled = order_info->qty_left;
                    send_msg(event::EventType::ORDER_CANCELED, order);
                    break;
                case XTP_ORDER_STATUS_REJECTED:
                    order->status = OrderStatus::Rejected;
                    send_msg(event::EventType::ORDER_REJECTED, order);
                    break;
                default:
                case XTP_ORDER_STATUS_UNKNOWN:
                    order->status = OrderStatus::Unknown;
                    send_msg(event::EventType::ORDER_ERROR, order);
                    break;
            }

            /*
            switch (order_info->order_submit_status) {
                case XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED: {
                    if (order_info->order_cancel_xtp_id != 0) {
                        //! 已撤单
                        order->status = OrderStatus::Cancelled;
                        send_msg(event::EventType::ORDER_CANCELLED, order);
                        LOG_PRINT_DBG("on order cancelled. {}", to_json(*order_info));
                    } else {
                        if (order_info->order_status == XTP_ORDER_STATUS_INIT
                            || order_info->order_status == XTP_ORDER_STATUS_NOTRADEQUEUEING) {
                            //! 已下单
                            order->status = OrderStatus::Accepted;
                            send_msg(event::EventType::ORDER_ACCEPTED, order);
                            LOG_PRINT_DBG("on order accepted. {}", to_json(*order_info));
                        } else if (order_info->order_status == XTP_ORDER_STATUS_PARTTRADEDQUEUEING) {
                            order->status = OrderStatus::PartialFilledActive;
                        }
                    }
                    break;
                }
                case XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED:
                    //! 已拒单
                    order->status = OrderStatus::Rejected;
                    send_msg(event::EventType::ORDER_REJECTED, order);
                    LOG_PRINT_DBG("on order rejected. {}", to_json(*order_info));
                    break;
                case XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED:
                    // FIXME: 这种情况暂没遇到过
                    order->status = OrderStatus::CancelAccepted;
                    send_msg(event::EventType::ORDER_CANCEL_ACCEPTED, order);
                    LOG_PRINT_DBG("on order cancel accepted. {}", to_json(*order_info));
                    break;
                case XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED:
                    // FIXME: 这种情况暂没遇到过
                    order->status = OrderStatus::CancelRejected;
                    send_msg(event::EventType::ORDER_CANCEL_REJECTED, order);
                    LOG_PRINT_DBG("on order cancel rejected. {}", to_json(*order_info));
                    break;
                default:
                    break;
            }
             */
        }

        void Trader::OnDisconnected(uint64_t session_id, int reason)
        {
            if (session_id == session_id_) {
                LOG_PRINT_ERR("disconnected, reason: {}, try to login again...", reason);
                auto s = trader_api_->Logout(session_id_);
                if (s == 0) {
                    LOG_PRINT_INFO("logout");
                    login();
                } else {
                    auto err = trader_api_->GetApiLastError();
                    LOG_PRINT_ERR("logout fail, (error_id={}) (error_msg={})", err->error_id, err->error_msg);
                }
            }
        }

        void Trader::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
        {
            LOG_PRINT_INFO(
                    "(symbol={}) (side={}) (price={}) (qty={}) (client_id={}) "
                    "(xtp_id={}) (xtp_cancel_id={}) (local_id={}) (order_status={}) (submit_status={})",
                    order_info->ticker, order_info->side, order_info->price, order_info->quantity,
                    order_info->order_client_id, order_info->order_xtp_id, order_info->order_cancel_xtp_id,
                    order_info->order_local_id, order_info->order_status, order_info->order_submit_status);

            if (order_info != nullptr) {
//                LOG_PRINT_INFO("xtp_order_info: {} session_id: {}", to_json(*order_info), session_id);
            }
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR(
                        "error_id: {} error_msg: {} session_id: {}",
                        error_info->error_id, error_info->error_msg, session_id
                );
            }

            on_order_info(order_info, error_info, session_id != session_id_);
        }

        void Trader::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
        {
            if (session_id != session_id_) {
                return;
            }

//            LOG_PRINT_INFO("trade_info: {}, (session_id={}), (session_id_={})",
//                           to_json(*trade_info), session_id, session_id_);

            auto trade = util::new_obj<type::Trade>();
            trade->volume = trade_info->quantity;
            trade->price = trade_info->price;
            trade->exchange = trade_info->market == XTP_MKT_SH_A
                              ? type::Exchange::SSE : type::Exchange::SZE;
            trade->symbol = {trade_info->ticker, trade->exchange};
            trade->side = trade_info->side == XTP_SIDE_BUY ? Side::BUY : Side::SELL;
            trade->trade_time = trade_info->trade_time;
            trade->trade_id = trade_info->order_xtp_id;
            strncpy(trade->account_id, get_account_id().c_str(), ACCOUNT_ID_LEN);

            send_msg(event::EventType::ORDER_TRADED, trade);
        }

        type::OrderInputSPtr Trader::insert_order(type::OrderInputSPtr const &order_input)
        {
            std::string msg = order_input->to_string();
            LOG_PRINT_DBG("{}", msg);

            XTPOrderInsertInfo info{};
            if (order_input->exchange == type::Exchange::SSE) {
                info.market = XTP_MKT_SH_A;
            } else if (order_input->exchange == type::Exchange::SZE) {
                info.market = XTP_MKT_SZ_A;
            } else {
                info.market = XTP_MKT_UNKNOWN;
            }
            strcpy(info.ticker, order_input->symbol);
            info.order_client_id = request_id_++;
            info.price = order_input->limit_price;
            info.quantity = order_input->qty;
            info.side = order_input->side == Side::BUY ? XTP_SIDE_BUY : XTP_SIDE_SELL;
            info.price_type = XTP_PRICE_LIMIT;
            info.business_type = XTP_BUSINESS_TYPE_CASH;
            order_input->order_id = info.order_client_id;

            uint64_t xtp_id = trader_api_->InsertOrder(&info, session_id_);
            if (xtp_id == 0) {
                XTPRI *error_info = trader_api_->GetApiLastError();
//
//                OrderActionError order_error{};
//                order_error.error_id = error_info->error_id;
//                strncpy(order_error.error_msg, error_info->error_msg, ERROR_MSG_LEN);
//                order_error.order_id = order_input->order_id;
//                order_error.xtp_order_id = xtp_id;
//
//                runner_->on_order_action_error(order_error);
                LOG_PRINT_ERR("(input){} (ErrorID){}, (ErrorMsg){}", msg, error_info->error_id, error_info->error_msg);
                return nullptr;
            } else {
                order_input->order_xtp_id = xtp_id;
//                Order order{};
//                order.order_id = order_input->order_id;
//                order.xtp_order_id = xtp_id;
//                order.insert_time = 0;
//                strncpy(order.trading_day, get_trading_day().c_str(), DATE_LEN);
//
//                runner_->on_order(order);
                LOG_PRINT_INFO("success to insert order, (order_id){} (xtp_order_id) {}", order_input->order_id,
                               xtp_id);
                return order_input;
            }
        }

        void Trader::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last,
                                  uint64_t session_id)
        {
            if (session_id != session_id_) {
                return;
            }

//            if (asset != nullptr) {
//                LOG_PRINT_INFO("asset: {}, request_id: {}, last: {}", to_json(*asset), request_id, is_last);
//            }
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("error_id: {}, error_msg: {}, request_id: {}, last: {}",
                              error_info->error_id, error_info->error_msg, request_id, is_last);
            }
            if (error_info == nullptr || error_info->error_id == 0 || error_info->error_id == 11000350) {
                type::Asset account{};
                if (error_info == nullptr || error_info->error_id == 0) {
                    account.avail = asset->buying_power;
                }
                strcpy(account.account_id, get_account_id().c_str());
                strcpy(account.trading_day, get_trading_day().c_str());
                account.update_time = -1;
                request_position();
            }
        }

        void Trader::OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last,
                                     uint64_t session_id)
        {
            if (session_id != session_id_) {
                return;
            }

//            if (position != nullptr) {
//                LOG_PRINT_INFO("position: {}, request_id: {}, last: {}", to_json(*position), request_id, is_last);
//            }
            if (error_info != nullptr && error_info->error_id != 0) {
                LOG_PRINT_ERR("error_id: {}, error_msg: {}, request_id: {}, last: {}",
                              error_info->error_id, error_info->error_msg, request_id, is_last);
            }
            if (error_info == nullptr || error_info->error_id == 0 || error_info->error_id == 11000350) {
                auto pos = util::new_obj<type::Position>();
                if (error_info == nullptr || error_info->error_id == 0) {
//                    strcpy(pos->symbol, position->ticker);
                    pos->exchange = position->market == XTP_MKT_SH_A
                                    ? type::Exchange::SSE : type::Exchange::SZE;
                    pos->symbol = {position->ticker, pos->exchange};
                    pos->volume = position->total_qty;
                    pos->yesterday_volume = position->sellable_qty;
                    pos->position_cost_price = position->avg_price;
                }
                strncpy(pos->account_id, get_account_id().c_str(), ACCOUNT_ID_LEN);
                strncpy(pos->trading_day, get_trading_day().c_str(), DATE_LEN);
                pos->direction = Direction::Long;
                pos->update_time = 0;
                if (is_last) {
                    /****/
                }

                send_msg(event::EventType::QRY_POS, pos);
            }
        }

        void Trader::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id)
        {
            if (session_id != session_id_) return;
            runner_->on_order_cancel_error(cancel_info->order_xtp_id, cancel_info->order_cancel_xtp_id);
        }

        bool Trader::cancel_order(uint64_t order_xtp_id)
        {
            auto cancel_xtp_id = trader_api_->CancelOrder(order_xtp_id, session_id_);
            if (cancel_xtp_id == 0) {
                auto *error_info = trader_api_->GetApiLastError();

                LOG_PRINT_ERR("failed to cancel order, (order_xtp_id={}) (session_id={}) (error_id={}) (error_msg={})",
                              order_xtp_id, session_id_, error_info->error_id, error_info->error_msg);
//                LOG_PRINT_ERR("failed to cancel order {}, (order_xtp_id={}) (session_id={}) (error_id={}) (error_msg={})",
//                        order->order_id, order->order_xtp_id, session_id_, error_info->error_id, error_info->error_msg);
                return false;
            }
            LOG_PRINT_INFO("success to cancel order, (order_xtp_id={}) (cancel_xtp_id={})",
                           order_xtp_id, cancel_xtp_id);
//            LOG_PRINT_INFO("success to cancel order {}, (order_xtp_id={}) (cancel_xtp_id={})",
//                     order->order_id, order->order_xtp_id, cancel_xtp_id);
            return true;
        }

        template<typename T>
        inline void Trader::send_td_msg(event::EventType type, T *data)
        {
            event::Event event{};
            event.type = type;
            event.data = reinterpret_cast<void *>(data);

            assert(td_queue_ != nullptr);
            td_queue_->enqueue(event);
        }

        void Trader::set_queue(const event::EventQueueSPtr &td_queue)
        {
            assert(td_queue != nullptr);
            td_queue_ = td_queue;
        }

        std::string Trader::name()
        {
            return "xtp";
        }

        std::string Trader::version()
        {
            return "2.2.33.5";
        }

        bool Trader::query_all_order()
        {
            XTPQueryOrderReq query_param{};
            auto s = trader_api_->QueryOrders(&query_param, session_id_, ++request_id_);
            if (s != 0) {
                auto err = trader_api_->GetApiLastError();
                LOG_PRINT_ERR("request query orders found error, error_id={}, error_msg={}",
                              err->error_id, err->error_msg);
                return false;
            }
            return true;
        }

        void Trader::OnQueryOrder(
                XTPQueryOrderRsp *order_info,
                XTPRI *error_info,
                int request_id,
                bool is_last,
                uint64_t session_id)
        {
            if (order_info == nullptr) return;

            on_order_info(order_info, error_info, session_id != session_id_);
        }
    }
}

