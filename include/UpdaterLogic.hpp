#pragma once

#include <string>
#include <vector>
#include <functional>

struct ReleaseInfo {
    std::string tag;
    std::string downloadUrl;
};

class UpdaterLogic {
public:
    static UpdaterLogic& getInstance();

    // Fetches the latest release info from the configured repo
    ReleaseInfo checkUpdate();

    // Downloads the ZIP file to a temporary location
    // Progress callback takes (downloaded_bytes, total_bytes)
    bool downloadUpdate(const std::string& url, std::function<void(size_t, size_t)> progressCallback);

    // Executes the clean logic and extracts the ZIP
    // Progress callback takes (current_file_index, total_files)
    bool installUpdate(bool rewrite, std::function<void(size_t, size_t)> progressCallback);

private:
    UpdaterLogic() = default;
    ~UpdaterLogic() = default;

    // Helper for glob pattern matching
    bool matchGlob(const std::string& pattern, const std::string& text);

    // Reads .updateignore from the root of the mock zip
    std::vector<std::string> readUpdateIgnore();
};
