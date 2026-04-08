#pragma once

#include <borealis.hpp>

class UpdateActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_DECLARE(UpdateActivity);

    UpdateActivity();
    ~UpdateActivity() override = default;

private:
    void onCheckUpdate(brls::View* view);
    void startUpdateProcess(bool rewrite);
    void performUpdate(bool rewrite);

    BRLS_BIND(brls::Button, btnCheckUpdate, "btnCheckUpdate");
    BRLS_BIND(brls::Label, lblStatus, "lblStatus");
    BRLS_BIND(brls::Button, btnRewrite, "btnRewrite");

    std::string downloadUrl;
};
