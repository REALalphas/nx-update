#include "ConfigManager.hpp"
#include <iostream>

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() {
    loadConfig();
}

AppConfig& ConfigManager::getConfig() {
    return config;
}

void ConfigManager::saveConfig() {
    // Mock save logic
    std::cout << "Saving config to /config/nx-updater/config.json" << std::endl;
}

void ConfigManager::loadConfig() {
    // Mock load logic
    std::cout << "Loading config from /config/nx-updater/config.json" << std::endl;
    // Set defaults as mock
    config.repoUrl = "https://github.com/REALalphas/nx";
    config.proxy.type = ProxyType::None;
}
