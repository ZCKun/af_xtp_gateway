cmake_minimum_required(VERSION 3.23)
project(xtp_ap)

set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME xtp_api)

include_directories(include/af include)
add_library(${TARGET_NAME} SHARED export.cc src/market_adapter.cc src/trader_adapter.cc)
target_include_directories(${TARGET_NAME}PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
