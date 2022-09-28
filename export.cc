//
// Created by 2h 0x on 2022/9/28.
//

#include "config.h"
#include "market_adapter.h"
#include "trader_adapter.h"

extern "C" __attribute__((externally_visible))
af::core::Market* create_market_gateway(toml::table const& cfg, af::strategy::Runner* runner)
{
    af::xtp::Config md_cfg{*cfg["md"].as_table()};
    return new af::xtp::Market(md_cfg, std::shared_ptr<af::strategy::Runner>(runner));
}

extern "C" __attribute__((externally_visible))
af::core::Trader* create_trader_gateway(toml::table const& cfg, af::strategy::Runner* runner)
{
    af::xtp::Config td_cfg{*cfg["td"].as_table()};
    return new af::xtp::Trader(td_cfg, std::shared_ptr<af::strategy::Runner>{runner});
}
