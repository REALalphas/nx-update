#include <borealis.hpp>
#include <switch.h>
#include "UpdateTab.hpp"
#include "SettingsTab.hpp"

int main(int argc, char* argv[]) {
    // Init libnx
    socketInitializeDefault();
    nxlinkStdio();

    // Init Borealis Logger
    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    // Init Borealis
    if (!brls::Application::init()) {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    brls::Application::createWindow("nx-update");

    // We create a TabFrame to hold our tabs
    brls::TabFrame* rootFrame = new brls::TabFrame();
    rootFrame->setTitle("nx-update");

    UpdateTab* updateTab = new UpdateTab();
    SettingsTab* settingsTab = new SettingsTab();

    rootFrame->addTab("Update", updateTab);
    rootFrame->addTab("Settings", settingsTab);

    brls::Application::pushView(rootFrame);

    // Main loop
    while (brls::Application::mainLoop()) {
        // App idle
    }

    // Exit
    socketExit();
    return EXIT_SUCCESS;
}
