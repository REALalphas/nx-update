#include "ConfigManager.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

// Temporary manual JSON parsing mock
// In a real app we would use nlohmann/json or similar
// For now, this is a mock implementation that just provides default settings
// since we haven't linked a JSON library yet

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

std::string ConfigManager::getConfigPath() const {
    return "/config/nx-updater/config.json";
}

void ConfigManager::load() {
    // Mock loading logic
    m_settings.repositoryUrl = "REALalphas/nx";
    m_settings.proxy.enabled = false;
    m_settings.proxy.type = "HTTP";
    m_settings.proxy.host = "";
    m_settings.proxy.port = 8080;
    m_settings.proxy.username = "";
    m_settings.proxy.password = "";
}

void ConfigManager::save() {
    // Mock saving logic
    std::cout << "Saving settings to " << getConfigPath() << std::endl;
}

Settings& ConfigManager::getSettings() {
    return m_settings;
}
