#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include "config.hpp"

class Network {
public:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    static std::string get(const std::string& url) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "nx-updater/1.0");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            // Proxy settings
            auto& config = Config::getInstance();
            if (!config.proxy.host.empty()) {
                curl_easy_setopt(curl, CURLOPT_PROXY, config.proxy.host.c_str());
                curl_easy_setopt(curl, CURLOPT_PROXYPORT, (long)config.proxy.port);

                if (config.proxy.type == "HTTP") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
                else if (config.proxy.type == "HTTPS") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
                else if (config.proxy.type == "SOCKS5") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

                if (!config.proxy.username.empty()) {
                    curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, config.proxy.username.c_str());
                    curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, config.proxy.password.c_str());
                }
            }

            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                return "";
            }
        }
        return readBuffer;
    }

    static bool downloadFile(const std::string& url, const std::string& path) {
        CURL* curl;
        FILE* fp;
        CURLcode res;

        curl = curl_easy_init();
        if (curl) {
            fp = fopen(path.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "nx-updater/1.0");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            // Proxy settings (same as get)
            auto& config = Config::getInstance();
            if (!config.proxy.host.empty()) {
                curl_easy_setopt(curl, CURLOPT_PROXY, config.proxy.host.c_str());
                curl_easy_setopt(curl, CURLOPT_PROXYPORT, (long)config.proxy.port);
                if (config.proxy.type == "HTTP") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
                else if (config.proxy.type == "HTTPS") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
                else if (config.proxy.type == "SOCKS5") curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
                if (!config.proxy.username.empty()) {
                    curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, config.proxy.username.c_str());
                    curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, config.proxy.password.c_str());
                }
            }

            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);

            return res == CURLE_OK;
        }
        return false;
    }
};
