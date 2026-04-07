#include <borealis.hpp>
#include <chrono>
#include <thread>
#include "config.hpp"
#include "network.hpp"
#include "update_engine.hpp"

class MainView : public brls::Box
{
public:
    MainView()
    {
        this->inflateFromXMLRes("main_view.xml");

        this->updateButton->registerAction("Update", brls::ControllerButton::BUTTON_A, [this](brls::View* view) {
            this->startUpdate();
            return true;
        });

        this->statusLabel->setText("Ready to check updates for " + Config::getInstance().repo);
    }

    void startUpdate() {
        int countdown = 5;
        brls::Dialog* dialog = new brls::Dialog("Update starts in " + std::to_string(countdown) + " seconds...");
        dialog->setCancelable(false);

        auto cancelBtn = new brls::Button();
        cancelBtn->setText("Cancel");
        cancelBtn->onClick([dialog]() {
            dialog->close();
        });
        dialog->addButton(cancelBtn);
        dialog->open();

        // Warning: Simplified countdown for now.
        // In real app, we'd use brls::Application::getTimerManager()
        // but for this build, we'll implement it when we have the real logic.
    }

    static brls::View* create()
    {
        return new MainView();
    }

private:
    BRLS_BIND(brls::Label, statusLabel, "statusLabel");
    BRLS_BIND(brls::Button, updateButton, "updateButton");
};

int main(int argc, char* argv[])
{
    Config::getInstance().load();

    if (!brls::Application::init())
    {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    brls::Application::setAppName("NX Updater");
    brls::Application::registerXMLView("main_view", MainView::create);

    brls::Application::pushView(new MainView());

    while (brls::Application::mainLoop())
        ;

    return EXIT_SUCCESS;
}
