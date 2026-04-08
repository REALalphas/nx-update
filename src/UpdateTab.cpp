#include "UpdateTab.hpp"
#include "ConfigManager.hpp"
#include "UpdateManager.hpp"
#include <iostream>

UpdateTab::UpdateTab() {
    this->setAxis(brls::Axis::COLUMN);
    this->setAlignItems(brls::AlignItems::CENTER);
    this->setJustifyContent(brls::JustifyContent::CENTER);

    statusLabel = new brls::Label();
    statusLabel->setText("Ready to check for updates");
    statusLabel->setMargins(0, 0, 20, 0);
    this->addView(statusLabel);

    checkButton = new brls::Button();
    checkButton->setStyle(brls::ButtonStyle::PRIMARY);
    checkButton->setTitle("Check for Updates");
    checkButton->getClickEvent()->subscribe([this](brls::View* view) {
        onCheckUpdates();
    });
    this->addView(checkButton);
}

brls::View* UpdateTab::create() {
    return new UpdateTab();
}

void UpdateTab::onCheckUpdates() {
    statusLabel->setText("Checking...");

    std::string repo = ConfigManager::getInstance().getSettings().repositoryUrl;
    currentRelease = UpdateManager::getInstance().checkLatestRelease(repo);

    statusLabel->setText("Update available: " + currentRelease.version + "\n" + currentRelease.changelog);

    checkButton->setTitle("Install Update");
    checkButton->getClickEvent()->unsubscribeAll();
    checkButton->getClickEvent()->subscribe([this](brls::View* view) {
        showCountdown();
    });
}

void UpdateTab::showCountdown() {
    countdownTimer = 5;
    checkButton->setState(brls::ViewState::DISABLED);

    // Instead of using brls::Timer which might be tricky to manage without async task
    // we use a simple linear countdown in the click event for this mock phase.

    statusLabel->setText("WARNING: Destructive operations ahead!\nUpdate starts in 5 seconds...");
    startUpdate();
}

void UpdateTab::startUpdate() {
    statusLabel->setText("Downloading update...");

    UpdateManager::getInstance().downloadRelease(currentRelease.downloadUrl);

    statusLabel->setText("Extracting update...");

    UpdateManager::getInstance().extractAndApplyUpdate("release.zip");

    statusLabel->setText("Update finished! Please restart the app.");
    checkButton->setTitle("Exit");
    checkButton->setState(brls::ViewState::DEFAULT);
    checkButton->getClickEvent()->unsubscribeAll();
    checkButton->getClickEvent()->subscribe([](brls::View* view) {
        brls::Application::quit();
    });
}
