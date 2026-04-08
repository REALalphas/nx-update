#pragma once

#include <borealis.hpp>

class MainActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_DECLARE(MainActivity);

    MainActivity();
    ~MainActivity() override = default;

private:
    BRLS_BIND(brls::Button, btnGoToUpdate, "btnGoToUpdate");
};
