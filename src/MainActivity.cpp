#include "MainActivity.hpp"
#include "UpdateActivity.hpp"

MainActivity::MainActivity() {
    this->btnGoToUpdate->registerClickAction([this](brls::View* view) {
        brls::Application::pushActivity(new UpdateActivity());
        return true;
    });
}
