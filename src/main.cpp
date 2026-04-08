#include <borealis.hpp>
#include "MainActivity.hpp"
#include "UpdateActivity.hpp"

// If building for Switch, libnx is available
#if defined(__SWITCH__)
#include <switch.h>
#else
// Mock romfsInit and romfsExit for host builds
void romfsInit() {}
void romfsExit() {}
#endif

int main(int argc, char* argv[]) {
    // Init romfs for XML layout loading
    romfsInit();

    // Init borealis
    if (!brls::Application::init("NX Updater")) {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    // Create main activity
    brls::Application::pushActivity(new MainActivity());

    // Run the main loop
    while (brls::Application::mainLoop()) {
        // ...
    }

    // Cleanup
    romfsExit();

    return EXIT_SUCCESS;
}
