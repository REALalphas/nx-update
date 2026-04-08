#pragma once
#include <borealis.hpp>

class SettingsTab : public brls::Box {
public:
    SettingsTab();
    static brls::View* create();
};
