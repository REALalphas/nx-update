#include "UpdateManager.hpp"
#include <iostream>

UpdateManager& UpdateManager::getInstance() {
    static UpdateManager instance;
    return instance;
}

ReleaseInfo UpdateManager::checkLatestRelease(const std::string& repo) {
    std::cout << "Checking for updates in repository: " << repo << std::endl;
    // Removed sleep_for to avoid freezing main thread in this mock

    ReleaseInfo info;
    info.version = "v1.0.1";
    info.downloadUrl = "https://github.com/" + repo + "/releases/download/v1.0.1/release.zip";
    info.changelog = "Mock release for testing UI.";
    return info;
}

bool UpdateManager::downloadRelease(const std::string& url) {
    std::cout << "Downloading from: " << url << std::endl;
    return true;
}

void UpdateManager::extractAndApplyUpdate(const std::string& zipPath) {
    std::cout << "Starting update process using " << zipPath << std::endl;

    // Simulating reading .updateignore from the zip
    std::vector<std::string> ignoreList = parseUpdateIgnore(".updateignore");

    // Mocking the clean process
    std::cout << "Cleaning directories..." << std::endl;
    cleanDirectory("/atmosphere/", ignoreList);

    std::cout << "Extracting new files..." << std::endl;
    std::cout << "Update finished!" << std::endl;
}

std::vector<std::string> UpdateManager::parseUpdateIgnore(const std::string& ignoreFilePath) {
    std::cout << "Parsing ignore file..." << std::endl;
    return {"atmosphere/config/*", "atmosphere/hosts/*"};
}

void UpdateManager::cleanDirectory(const std::string& dirPath, const std::vector<std::string>& ignoreList) {
    std::cout << "Cleaning " << dirPath << " but preserving items in updateignore..." << std::endl;
    for (const auto& rule : ignoreList) {
        std::cout << "  - Preserved: " << rule << std::endl;
    }
}
