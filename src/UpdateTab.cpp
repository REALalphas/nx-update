#include "UpdateTab.hpp"
#include "ConfigManager.hpp"
#include "UpdateManager.hpp"
#include <iostream>
#include <thread>

UpdateTab::UpdateTab() {
    this->setFlexDirection(brls::FlexDirection::COLUMN);
    this->setAlignItems(brls::AlignItems::CENTER);
    this->setJustifyContent(brls::JustifyContent::CENTER);

    statusLabel = new brls::Label();
    statusLabel->setText("Ready to check for updates");
    statusLabel->setMargins(0, 0, 20, 0);
    this->addView(statusLabel);

    actionButton = new brls::Button(brls::ButtonStyle::PRIMARY);
    actionButton->setTitle("Check for Updates");

    actionButton->getClickEvent()->subscribe([this](brls::View* view) {
        updateButtonAction();
    });
    this->addView(actionButton);
}

brls::View* UpdateTab::create() {
    return new UpdateTab();
}

void UpdateTab::updateButtonAction() {
    switch (currentState) {
        case State::Idle:
            onCheckUpdates();
            break;
        case State::UpdateAvailable:
            showCountdown();
            break;
        case State::Finished:
            brls::Application::quit();
            break;
        default:
            break;
    }
}

void UpdateTab::onCheckUpdates() {
    statusLabel->setText("Checking...");

    std::string repo = ConfigManager::getInstance().getSettings().repositoryUrl;
    currentRelease = UpdateManager::getInstance().checkLatestRelease(repo);

    statusLabel->setText("Update available: " + currentRelease.version + "\n" + currentRelease.changelog);
    actionButton->setTitle("Install Update");

    currentState = State::UpdateAvailable;
}

void UpdateTab::showCountdown() {
    currentState = State::Countdown;
    actionButton->setInteractable(false);
    countdownValue = 5;

    statusLabel->setText("WARNING: Destructive operations ahead!\nUpdate starts in " + std::to_string(countdownValue.load()) + " seconds...");

    brls::async([this]() {
        for (int i = 5; i > 0; --i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            brls::sync([this]() {
                if (currentState != State::Countdown) return;

                countdownValue--;
                if (countdownValue.load() > 0) {
                    statusLabel->setText("WARNING: Destructive operations ahead!\nUpdate starts in " + std::to_string(countdownValue.load()) + " seconds...");
                } else {
                    startUpdate();
                }
            });
        }
    });
}

void UpdateTab::startUpdate() {
    currentState = State::Finished;

    brls::async([this]() {
        brls::sync([this]() {
            statusLabel->setText("Downloading update...");
        });

        UpdateManager::getInstance().downloadRelease(currentRelease.downloadUrl);

        brls::sync([this]() {
            statusLabel->setText("Extracting update...");
        });

        UpdateManager::getInstance().extractAndApplyUpdate("release.zip");

        brls::sync([this]() {
            statusLabel->setText("Update finished! Please restart the app.");
            actionButton->setTitle("Exit");
            actionButton->setInteractable(true);
        });
    });
}
