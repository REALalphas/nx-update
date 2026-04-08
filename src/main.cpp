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

    rootFrame->addTab("Update", UpdateTab::create);
    rootFrame->addTab("Settings", SettingsTab::create);

    brls::Application::pushActivity(new brls::Activity(rootFrame));

    // Main loop
    while (brls::Application::mainLoop()) {
        // App idle
    }

    // Exit
    socketExit();
    return EXIT_SUCCESS;
}
