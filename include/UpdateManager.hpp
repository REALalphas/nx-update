#pragma once

#include <string>
#include <vector>

struct ReleaseInfo {
    std::string version;
    std::string downloadUrl;
    std::string changelog;
};

class UpdateManager {
public:
    static UpdateManager& getInstance();

    ReleaseInfo checkLatestRelease(const std::string& repo);
    bool downloadRelease(const std::string& url);
    void extractAndApplyUpdate(const std::string& zipPath);

private:
    UpdateManager() = default;

    // Mock helper for update logic
    std::vector<std::string> parseUpdateIgnore(const std::string& ignoreFilePath);
    void cleanDirectory(const std::string& dirPath, const std::vector<std::string>& ignoreList);
};
