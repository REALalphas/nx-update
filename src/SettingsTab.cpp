#include "SettingsTab.hpp"
#include "ConfigManager.hpp"
#include <iostream>

SettingsTab::SettingsTab() {
    this->setFlexDirection(brls::FlexDirection::COLUMN);
    this->setPadding(20, 20, 20, 20);

    // Repository Section
    brls::Label* repoHeader = new brls::Label();
    repoHeader->setText("GitHub Repository");
    repoHeader->setFontSize(24);
    repoHeader->setMargins(0, 0, 10, 0);
    this->addView(repoHeader);

    brls::Button* repoBtn = new brls::Button(brls::ButtonStyle::REGULAR);
    repoBtn->setTitle(ConfigManager::getInstance().getSettings().repositoryUrl);
    repoBtn->getClickEvent()->subscribe([](brls::View* view) {
        std::cout << "Open keyboard to edit repository" << std::endl;
    });
    this->addView(repoBtn);

    // Proxy Section
    brls::Label* proxyHeader = new brls::Label();
    proxyHeader->setText("Proxy Settings");
    proxyHeader->setFontSize(24);
    proxyHeader->setMargins(20, 0, 10, 0);
    this->addView(proxyHeader);

    brls::Button* proxyTypeBtn = new brls::Button(brls::ButtonStyle::REGULAR);
    proxyTypeBtn->setTitle("Type: " + ConfigManager::getInstance().getSettings().proxy.type);
    proxyTypeBtn->getClickEvent()->subscribe([](brls::View* view) {
        std::cout << "Cycle proxy type (HTTP/HTTPS/SOCKS5)" << std::endl;
    });
    this->addView(proxyTypeBtn);

    brls::Button* proxyHostBtn = new brls::Button(brls::ButtonStyle::REGULAR);
    proxyHostBtn->setTitle("Host: " + (ConfigManager::getInstance().getSettings().proxy.host.empty() ? "None" : ConfigManager::getInstance().getSettings().proxy.host));
    this->addView(proxyHostBtn);

    brls::Button* proxyPortBtn = new brls::Button(brls::ButtonStyle::REGULAR);
    proxyPortBtn->setTitle("Port: " + std::to_string(ConfigManager::getInstance().getSettings().proxy.port));
    this->addView(proxyPortBtn);
}

brls::View* SettingsTab::create() {
    return new SettingsTab();
}
