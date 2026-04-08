#pragma once

#include <string>

enum class ProxyType {
    None,
    HTTP,
    HTTPS,
    SOCKS5
};

struct ProxyConfig {
    ProxyType type = ProxyType::None;
    std::string host = "";
    int port = 0;
};

struct AppConfig {
    std::string repoUrl = "https://github.com/REALalphas/nx";
    ProxyConfig proxy;
};

class ConfigManager {
public:
    static ConfigManager& getInstance();

    AppConfig& getConfig();
    void saveConfig();
    void loadConfig();

private:
    ConfigManager();
    ~ConfigManager() = default;

    AppConfig config;
};
