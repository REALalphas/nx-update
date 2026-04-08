#pragma once

#include <string>

struct ProxySettings {
    bool enabled = false;
    std::string type = "HTTP"; // HTTP, HTTPS, SOCKS5
    std::string host;
    int port = 8080;
    std::string username;
    std::string password;
};

struct Settings {
    std::string repositoryUrl = "REALalphas/nx";
    ProxySettings proxy;
};

class ConfigManager {
public:
    static ConfigManager& getInstance();

    void load();
    void save();

    Settings& getSettings();

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    Settings m_settings;
    std::string getConfigPath() const;
};
