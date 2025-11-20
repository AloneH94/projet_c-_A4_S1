#pragma once
#include "Option.h"

class AmericanOption : public Option {
public:
    AmericanOption(double expiry) : Option(expiry) {}
    bool isAmericanOption() const override { return true; }
};
