//
// Created by x2h1z on 22-5-9.
//
#pragma once

#include <string>
#include <toml.hpp>
#include <af/utils/common.h>
#include <string>
#include "log.h"

namespace af
{
    namespace xtp
    {
        struct Config
        {
            int32_t client_id = 0;
            std::string username;
            std::string password;
            std::string software_key;
            std::string md_ip;
            int32_t md_port = 0;
            std::string td_ip;
            int32_t td_port = 0;
            std::string protocol;
            int32_t buffer_size = 0;

            Config() = default;

            explicit Config(toml::table const &cfg)
            {
                init(cfg);
            }

            explicit Config(std::string const &file_path) :
                    Config(toml::parse_file(file_path))
            {}

            void init(toml::table const &config)
            {
                client_id = config["client_id"].value_or(0);
                if (client_id > 100) {
                    LOG_PRINT_WRN("(client_id={}) 大于 100 可能会出现问题", client_id);
                }
                username = config["username"].value_or("");
                if (username.empty()) {
                    LOG_PRINT_ERR("(username={}) 请配置用户名", username);
                    exit(EXIT_FAILURE);
                }
                std::string pass = config["password"].value_or("");
                if (pass.empty()) {
                    LOG_PRINT_ERR("(password={}) 请配置密码", pass);
                    exit(EXIT_FAILURE);
                }

//                u_char buf[1024] = {'\0'};
//                u_char result[1024] = {'\0'};
//                auto len = util::hex_to_bytes(pass, buf);
//            util::aes_decrypt(buf, len, result, k, v);
//                password = (char *) result;

                software_key = config["software_key"].value_or("");
                if (software_key.empty()) {
                    LOG_PRINT_ERR("(software_key={}) 请配置software key", software_key);
                    exit(EXIT_FAILURE);
                }
                md_ip = config["md_ip"].value_or("");
                if (md_ip.empty()) {
                    LOG_PRINT_ERR("(md_ip={}) 请配置行情IP", md_ip);
                    exit(EXIT_FAILURE);
                }
                md_port = config["md_port"].value_or(-1);
                if (md_port > 65535 or md_port < 0) {
                    LOG_PRINT_ERR("(md_port={}) 请配置正确的行情端口(0~65535)", md_port);
                    exit(EXIT_FAILURE);
                }
                td_ip = config["td_ip"].value_or("");
                if (td_ip.empty()) {
                    LOG_PRINT_ERR("(td_ip={}) 请配置交易IP", td_ip);
                    exit(EXIT_FAILURE);
                }
                td_port = config["td_port"].value_or(-1);
                if (td_port > 65535 or td_port < 0) {
                    LOG_PRINT_ERR("(td_port={}) 请配置正确的交易端口(0~65535)", td_port);
                    exit(EXIT_FAILURE);
                }
                protocol = config["protocol"].value_or("");
                if (protocol.empty() or (protocol != "udp" and protocol != "tcp")) {
                    LOG_PRINT_ERR("(protocol={}) 请配置正确的协议(udp/tcp)", protocol);
                    exit(EXIT_FAILURE);
                }
                buffer_size = config["buffer_size"].value_or(1024);

            }
        };

    }
}
