#pragma once
#include <borealis.hpp>
#include "UpdateManager.hpp"
#include <atomic>

class UpdateTab : public brls::Box {
public:
    UpdateTab();
    static brls::View* create();
private:
    void onCheckUpdates();
    void showCountdown();
    void startUpdate();
    void updateButtonAction();

    brls::Label* statusLabel;
    brls::Button* actionButton;
    ReleaseInfo currentRelease;
    std::atomic<int> countdownValue{5};

    enum class State {
        Idle,
        UpdateAvailable,
        Countdown,
        Finished
    };
    State currentState = State::Idle;
};
