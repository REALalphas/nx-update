#pragma once
#include <string>
#include "../external/nlohmann/json.hpp"
#include "network.hpp"

class GitHub {
public:
    struct Release {
        std::string tag;
        std::string zipUrl;
    };

    static Release getLatestRelease(const std::string& repo) {
        std::string url = "https://api.github.com/repos/" + repo + "/releases/latest";
        std::string response = Network::get(url);

        Release release;
        if (!response.empty()) {
            auto j = nlohmann::json::parse(response);
            release.tag = j.value("tag_name", "");
            for (auto& asset : j["assets"]) {
                std::string name = asset.value("name", "");
                if (name.find(".zip") != std::string::npos) {
                    release.zipUrl = asset.value("browser_download_url", "");
                    break;
                }
            }
        }
        return release;
    }
};
