#pragma once
#include <string>
#include <vector>
#include "../external/nlohmann/json.hpp"
#include <fstream>
#include <filesystem>

struct ProxyConfig {
    std::string host;
    int port = 0;
    std::string type; // HTTP, HTTPS, SOCKS5
    std::string username;
    std::string password;
};

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    void load() {
        if (!std::filesystem::exists(configPath)) {
            save(); // Create default config
            return;
        }

        try {
            std::ifstream file(configPath);
            nlohmann::json j;
            file >> j;

            repo = j.value("repo", "REALalphas/nx");
            proxy.host = j["proxy"].value("host", "");
            proxy.port = j["proxy"].value("port", 0);
            proxy.type = j["proxy"].value("type", "HTTP");
            proxy.username = j["proxy"].value("username", "");
            proxy.password = j["proxy"].value("password", "");

            rewrite = j.value("rewrite", false);

        } catch (...) {
            // Handle error or use defaults
        }
    }

    void save() {
        nlohmann::json j;
        j["repo"] = repo;
        j["proxy"]["host"] = proxy.host;
        j["proxy"]["port"] = proxy.port;
        j["proxy"]["type"] = proxy.type;
        j["proxy"]["username"] = proxy.username;
        j["proxy"]["password"] = proxy.password;
        j["rewrite"] = rewrite;

        std::filesystem::create_directories(std::filesystem::path(configPath).parent_path());
        std::ofstream file(configPath);
        file << j.dump(4);
    }

    std::string repo = "REALalphas/nx";
    ProxyConfig proxy;
    bool rewrite = false;

private:
    Config() = default;
    const std::string configPath = "/config/nx-updater/config.json";
};
