#pragma once
#include <borealis.hpp>
#include "UpdateManager.hpp"

class UpdateTab : public brls::Box {
public:
    UpdateTab();
    static brls::View* create();
private:
    void onCheckUpdates();
    void showCountdown();
    void startUpdate();

    brls::Label* statusLabel;
    brls::Button* checkButton;
    ReleaseInfo currentRelease;
    int countdownTimer = 5;
};
