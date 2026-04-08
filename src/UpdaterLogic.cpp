#include "UpdaterLogic.hpp"
#include "ConfigManager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

UpdaterLogic& UpdaterLogic::getInstance() {
    static UpdaterLogic instance;
    return instance;
}

ReleaseInfo UpdaterLogic::checkUpdate() {
    // Mock update check
    std::cout << "Checking update from: " << ConfigManager::getInstance().getConfig().repoUrl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return {"v1.2.3", "https://example.com/update.zip"};
}

bool UpdaterLogic::downloadUpdate(const std::string& url, std::function<void(size_t, size_t)> progressCallback) {
    // Mock download
    std::cout << "Downloading update from: " << url << std::endl;
    size_t total = 100;
    for (size_t i = 0; i <= total; i += 10) {
        if (progressCallback) progressCallback(i, total);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return true;
}

bool UpdaterLogic::matchGlob(const std::string& pattern, const std::string& text) {
    // Simple mock glob matcher (only handles * at the end for basic testing)
    if (pattern.back() == '*') {
        std::string prefix = pattern.substr(0, pattern.length() - 1);
        return text.find(prefix) == 0;
    }
    return pattern == text;
}

std::vector<std::string> UpdaterLogic::readUpdateIgnore() {
    // Mock reading .updateignore
    return {"atmosphere/config/*", "keep_this_file.txt"};
}

bool UpdaterLogic::installUpdate(bool rewrite, std::function<void(size_t, size_t)> progressCallback) {
    std::cout << "Installing update..." << std::endl;

    // Mock extraction process
    std::vector<std::string> zipContents = {"atmosphere/package1", "atmosphere/config/system_settings.ini", "bootloader/hekate.bin"};
    std::vector<std::string> sdContents = {"atmosphere/old_package", "atmosphere/config/custom_settings.ini", "games/my_game"};

    std::vector<std::string> ignoreList;
    if (!rewrite) {
        ignoreList = readUpdateIgnore();
    }

    std::cout << "Cleaning up old files..." << std::endl;
    for (const auto& itemInZip : zipContents) {
        // Mock checking if item exists in zip root to determine what to delete
        // In real logic, we'd list the zip root directories/files and delete matching items on SD card.
    }

    std::cout << "Extracting new files..." << std::endl;
    size_t totalFiles = zipContents.size();
    for (size_t i = 0; i < totalFiles; ++i) {
        std::cout << "Extracting: " << zipContents[i] << std::endl;
        if (progressCallback) progressCallback(i + 1, totalFiles);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Mock Self-Update mechanism
    std::cout << "Replacing self (.nro)... Backup created with .bkp extension" << std::endl;

    return true;
}
