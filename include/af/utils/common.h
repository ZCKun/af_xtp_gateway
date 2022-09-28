//
// Created by 2h 0x on 2022/5/11.
//
#pragma once

#include <fmt/format.h>
#include <chrono>
#include <fmt/chrono.h>
#include <utility>
#include <climits> /* INT_MIN, INT_MAX */
#include <unordered_map>
#include <set>


namespace af
{
    namespace util
    {
        static inline int32_t hex_to_bytes(std::string const &hex, u_char *ret);

        static inline int32_t aes_decrypt(
                u_char *ciphertext,
                int32_t ciphertext_len,
                u_char *plaintext,
                u_char *key = nullptr,
                u_char *iv = nullptr);

        static inline uint64_t rdtsc()
        {
            uint64_t ret;
#ifdef __x86_64__
            ret = __builtin_ia32_rdtsc();
#elif __arm__
            asm volatile("mrs %0, cntvct_el0": "=r"(ret));
#endif
            return ret;
        }

        inline bool is_sz_stock(std::string const &symbol_code)
        {
            auto s = symbol_code.c_str();
            return (s[0] == '0' || s[0] == '3') && s[1] == '0';
        }

        inline bool is_sh_stock(std::string const &symbol_code)
        {
            auto s = symbol_code.c_str();
            return s[0] == '6' && (s[1] == '0' || s[1] == '8');
        }

        inline bool is_sz_bond(std::string const &symbol_code)
        {
            auto s = symbol_code.c_str();
            return s[0] == '1' && s[1] == '2';
        }

        inline bool is_sh_bond(std::string const &symbol_code)
        {
            auto s = symbol_code.c_str();
            return s[0] == '1' && s[1] == '1';
        }

        inline bool is_stock(std::string const &symbol_code)
        {
            return is_sh_stock(symbol_code) && is_sz_stock(symbol_code);
        }

        inline bool is_bond(std::string const &symbol_code)
        {
            return is_sh_bond(symbol_code) && is_sz_bond(symbol_code);
        }

        /**
         * 获取当前系统时间
         * @tparam T 返回时间格式
         * @return
         */
        template<typename T>
        inline int64_t now_time()
        {
            return std::chrono::duration_cast<T>(
                    std::chrono::system_clock::now().time_since_epoch()
            ).count();
        }

        /**
         * 获取当前系统时间纳秒数
         * @return
         */
        inline int64_t now_ns()
        {
            return now_time<std::chrono::nanoseconds>();
        }

        /**
         * 获取当前系统时间毫秒数
         * @return
         */
        inline int64_t now_ms()
        {
            return now_time<std::chrono::milliseconds>();
        }

        /**
         * 获取当前系统时间微秒数
         * @return
         */
        inline int64_t now_us()
        {
            return now_time<std::chrono::microseconds>();
        }

        /* code from http://git.musl-libc.org/cgit/musl/tree/src/time/__secs_to_tm.c?h=v0.9.15 */
        /* 2000-03-01 (mod 400 year, immediately after feb29 */
#define LEAPOCH (946684800LL + 86400*(31+29))

#define DAYS_PER_400Y (365*400 + 97)
#define DAYS_PER_100Y (365*100 + 24)
#define DAYS_PER_4Y   (365*4   + 1)

        [[maybe_unused]] inline int secs_to_tm(long long t, struct tm *tm)
        {
            long long days, secs;
            int remdays, remsecs, remyears;
            int qc_cycles, c_cycles, q_cycles;
            int years, months;
            int wday, yday, leap;
            static const char days_in_month[] = {31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 29};

            /* Reject time_t values whose year would overflow int */
            if (t < INT_MIN * 31622400LL || t > INT_MAX * 31622400LL)
                return -1;

            secs = t - LEAPOCH;
            days = secs / 86400;
            remsecs = static_cast<int>(secs % 86400);
            if (remsecs < 0) {
                remsecs += 86400;
                days--;
            }

            wday = static_cast<int>((3 + days) % 7);
            if (wday < 0) wday += 7;

            qc_cycles = static_cast<int>(days / DAYS_PER_400Y);
            remdays = static_cast<int>(days % DAYS_PER_400Y);
            if (remdays < 0) {
                remdays += DAYS_PER_400Y;
                qc_cycles--;
            }

            c_cycles = remdays / DAYS_PER_100Y;
            if (c_cycles == 4) c_cycles--;
            remdays -= c_cycles * DAYS_PER_100Y;

            q_cycles = remdays / DAYS_PER_4Y;
            if (q_cycles == 25) q_cycles--;
            remdays -= q_cycles * DAYS_PER_4Y;

            remyears = remdays / 365;
            if (remyears == 4) remyears--;
            remdays -= remyears * 365;

            leap = !remyears && (q_cycles || !c_cycles);
            yday = remdays + 31 + 28 + leap;
            if (yday >= 365 + leap) yday -= 365 + leap;

            years = remyears + 4 * q_cycles + 100 * c_cycles + 400 * qc_cycles;

            for (months = 0; days_in_month[months] <= remdays; months++)
                remdays -= days_in_month[months];

            if (years + 100 > INT_MAX || years + 100 < INT_MIN)
                return -1;

            tm->tm_year = years + 100;
            tm->tm_mon = months + 2;
            if (tm->tm_mon >= 12) {
                tm->tm_mon -= 12;
                tm->tm_year++;
            }
            tm->tm_mday = remdays + 1;
            tm->tm_wday = wday;
            tm->tm_yday = yday;

            tm->tm_hour = remsecs / 3600;
            tm->tm_min = remsecs / 60 % 60;
            tm->tm_sec = remsecs % 60;

            return 0;
        }

        struct Milliseconds
        {
            static inline std::tuple<int64_t, std::string> parse(int64_t ms)
            {
                int64_t t = ms / 1'000;
                int64_t suffix = ms % 1'000;
                std::string suffix_s;
                if (suffix - 1 == -1) { suffix_s = "000"; }
                else if (suffix - 10 < 0) { suffix_s = fmt::format("00{}", suffix); }
                else if (suffix - 100 < 0) { suffix_s = fmt::format("0{}", suffix); }
                else { suffix_s = std::to_string(suffix); }

                return std::make_tuple(t, suffix_s);
            }
        };

        struct Microseconds
        {
            static inline std::tuple<int64_t, std::string> parse(int64_t ms)
            {
                std::string suffix_s;
                int64_t t = ms / 1'000'000;
                int64_t suffix = ms % 1'000'000;

                if (suffix - 1 == -1) { suffix_s = "000000"; }
                else if (suffix - 10 < 0) { suffix_s = fmt::format("00000{}", suffix); }
                else if (suffix - 100 < 0) { suffix_s = fmt::format("0000{}", suffix); }
                else if (suffix - 1'000 < 0) { suffix_s = fmt::format("000{}", suffix); }
                else if (suffix - 10'000 < 0) { suffix_s = fmt::format("00{}", suffix); }
                else if (suffix - 100'000 < 0) { suffix_s = fmt::format("0{}", suffix); }
                else { suffix_s = std::to_string(suffix); }

                return std::make_tuple(t, suffix_s);
            }
        };

        struct Nanoseconds
        {
            static inline std::tuple<int64_t, std::string> parse(int64_t ms)
            {
                std::string suffix_s;
                int64_t t = ms / 1'000'000'000;
                int64_t suffix = ms % 1'000'000'000;

                if (suffix - 1 == -1) { suffix_s = "000000000"; }
                else if (suffix - 10 < 0) { suffix_s = fmt::format("00000000{}", suffix); }
                else if (suffix - 100 < 0) { suffix_s = fmt::format("0000000{}", suffix); }
                else if (suffix - 1'000 < 0) { suffix_s = fmt::format("000000{}", suffix); }
                else if (suffix - 10'000 < 0) { suffix_s = fmt::format("00000{}", suffix); }
                else if (suffix - 100'000 < 0) { suffix_s = fmt::format("0000{}", suffix); }
                else if (suffix - 1'000'000 < 0) { suffix_s = fmt::format("000{}", suffix); }
                else if (suffix - 10'000'000 < 0) { suffix_s = fmt::format("00{}", suffix); }
                else if (suffix - 100'000'000 < 0) { suffix_s = fmt::format("0{}", suffix); }
                else { suffix_s = std::to_string(suffix); }

                return std::make_tuple(t, suffix_s);
            }
        };

        /*!
         * @brief 转换微秒级的 unix time 为time
         * @param ms 待转换的 unix time
         * @param save_ns 是否保留纳秒部分
         * @return
        */
        template<typename T>
        inline int64_t convert_ts_to_time(int64_t ms, T p)
        {
            auto [t, suffix_s] = p.parse(ms);

            std::tm tm{};
            if (
                    secs_to_tm(t, &tm
                    ) == 0) {
                const char *min_append = (tm.tm_min - 10 < 0) ? "0" : "";
                const char *sec_append = (tm.tm_sec - 10 < 0) ? "0" : "";
                auto time_str = fmt::format("{}{}{}{}{}{}", tm.tm_hour + 8, min_append, tm.tm_min, sec_append,
                                            tm.tm_sec,
                                            suffix_s);
                return
                        std::stoll(time_str);
            }

            return -1;
        }

        inline int64_t now_time()
        {
            return convert_ts_to_time(now_ms(), Milliseconds());
        }

        inline std::string time_to_str(int64_t time_diff)
        {
            int64_t second_ms = 1'000;
            int64_t minute_ms = second_ms * 60;

            std::string time_diff_str;
            if (time_diff >= minute_ms) {
                double time_diff_min = static_cast<double>(time_diff) / static_cast<double>(minute_ms);
                time_diff_str = fmt::format(
                        FMT_STRING("{:.2f}min"),
                        time_diff_min
                );
            } else if (time_diff >= second_ms) {
                double time_diff_s = static_cast<double>(time_diff) / static_cast<double>(second_ms);
                time_diff_str = fmt::format(
                        FMT_STRING("{:.2f}s"),
                        time_diff_s
                );
            } else {
                time_diff_str = fmt::format(
                        FMT_STRING("{}ms"),
                        time_diff
                );
            }

            return time_diff_str;
        }
    }
}
