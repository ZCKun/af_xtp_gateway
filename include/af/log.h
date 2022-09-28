//
// Created by x2h1z on 22-5-9.
//
#pragma once
#define FMTLOG_ACTIVE_LEVEL FMTLOG_LEVEL_DBG

#include <fmt/format.h>
#include <cstdio>
#include <thread>
#include <filesystem>
#include <sstream>

#if (defined(DEBUG))
#define __FILENAME__ __FILE__
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

/** Formatting codes **/
#define RESET "\033[m"
#define BOLD "\033[1m"
#define DARK "\033[2m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define CONCEALED "\033[8m"
#define CLEAR_LINE "\033[K"

/** Foreground colors **/
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

/** Background colors **/
#define ON_BLACK "\033[40m"
#define ON_RED "\033[41m"
#define ON_GREEN "\033[42m"
#define ON_YELLOW "\033[43m"
#define ON_BLUE "\033[44m"
#define ON_MAGENTA "\033[45m"
#define ON_CYAN "\033[46m"
#define ON_WHITE "\033[47m"

#define LOG(tag, color, ...) \
    printf("%s", fmt::format(         \
        FMT_STRING("[{}:{}{}{}:{}] {}{}: {}{}\n"), \
        __FILENAME__,                  \
        WHITE,      \
        __func__,   \
        RESET,      \
        __LINE__,   \
        color,          \
        tag,          \
        fmt::format(__VA_ARGS__).c_str(), \
        RESET       \
    ).c_str());

#define LOG_PRINT_INFO(...) LOG("INFO", GREEN, __VA_ARGS__)
//#if DEBUG
#define LOG_PRINT_DBG(...) LOG("DEBUG", CYAN, __VA_ARGS__)
//#else
//#define LOG_PRINT_DBG(...)
//#endif
#define LOG_PRINT_ERR(...) LOG("ERROR", RED, __VA_ARGS__)
#define LOG_PRINT_WRN(...) LOG("WARNER", YELLOW, __VA_ARGS__)

namespace af
{
    namespace log
    {
        enum class Level
        {
            DBG = 0,
            INF,
            WRN,
            ERR,
            OFF
        };

        inline std::string get_today_date()
        {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            auto tt = std::localtime(&in_time_t);
            std::stringstream ss;
            ss << std::put_time(tt, "%Y%m%d");

            return ss.str();
        }

        struct Log
        {
            static int init();
        };
    }
}

//#define LOG_PRINT_DBG(...) logd(__VA_ARGS__)
//#define LOG_PRINT_INFO(...) logi(__VA_ARGS__)
//#define LOG_PRINT_WRN(...) logw(__VA_ARGS__)
//#define LOG_PRINT_ERR(...) loge(__VA_ARGS__)
//#define LOG_DBG(...) logwd(__VA_ARGS__)
//#define LOG_INF(...) logwi(__VA_ARGS__)
//#define LOG_WRN(...) logww(__VA_ARGS__)
//#define LOG_ERR(...) logwe(__VA_ARGS__)
