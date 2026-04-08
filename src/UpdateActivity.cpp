#include "UpdateActivity.hpp"
#include "UpdaterLogic.hpp"
#include <iostream>
#include <thread>
#include <chrono>

UpdateActivity::UpdateActivity() {
    this->btnCheckUpdate->registerClickAction([this](brls::View* view) {
        this->onCheckUpdate(view);
        return true;
    });

    this->btnRewrite->registerClickAction([this](brls::View* view) {
        this->startUpdateProcess(true);
        return true;
    });
    this->btnRewrite->setVisibility(brls::Visibility::GONE);
}

void UpdateActivity::onCheckUpdate(brls::View* view) {
    this->lblStatus->setText("Checking for updates...");
    this->btnCheckUpdate->setDisabled(true);

    brls::async([this]() {
        ReleaseInfo info = UpdaterLogic::getInstance().checkUpdate();

        brls::sync([this, info]() {
            if (!info.tag.empty()) {
                this->lblStatus->setText("Found update: " + info.tag);
                this->downloadUrl = info.downloadUrl;

                // Change button to Start Update
                this->btnCheckUpdate->setText("Install Update (Keep Data)");
                this->btnCheckUpdate->setDisabled(false);
                this->btnCheckUpdate->registerClickAction([this](brls::View* view) {
                    this->startUpdateProcess(false);
                    return true;
                });

                this->btnRewrite->setVisibility(brls::Visibility::VISIBLE);
            } else {
                this->lblStatus->setText("No updates found.");
                this->btnCheckUpdate->setDisabled(false);
            }
        });
    });
}

void UpdateActivity::startUpdateProcess(bool rewrite) {
    this->btnCheckUpdate->setDisabled(true);
    this->btnRewrite->setDisabled(true);

    brls::Application::getGlobalCoreEvent()->schedule([this, rewrite]() {
        // 5-second countdown warning
        brls::async([this, rewrite]() {
            for (int i = 5; i > 0; --i) {
                brls::sync([this, i]() {
                    this->lblStatus->setText("WARNING: Destructive operation in " + std::to_string(i) + " seconds...");
                });
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            this->performUpdate(rewrite);
        });
    });
}

void UpdateActivity::performUpdate(bool rewrite) {
    brls::sync([this]() {
        this->lblStatus->setText("Downloading update...");
    });

    bool success = UpdaterLogic::getInstance().downloadUpdate(this->downloadUrl, [this](size_t downloaded, size_t total) {
        brls::sync([this, downloaded, total]() {
            this->lblStatus->setText("Downloading: " + std::to_string(downloaded) + " / " + std::to_string(total));
        });
    });

    if (success) {
        brls::sync([this]() {
            this->lblStatus->setText("Installing update...");
        });

        UpdaterLogic::getInstance().installUpdate(rewrite, [this](size_t current, size_t total) {
            brls::sync([this, current, total]() {
                this->lblStatus->setText("Extracting: " + std::to_string(current) + " / " + std::to_string(total));
            });
        });

        brls::sync([this]() {
            this->lblStatus->setText("Update complete! Please restart.");
        });
    } else {
        brls::sync([this]() {
            this->lblStatus->setText("Update failed.");
            this->btnCheckUpdate->setDisabled(false);
            this->btnRewrite->setDisabled(false);
        });
    }
}
