#pragma once
#include "AmericanOption.h"
#include <algorithm>

class AmericanCallOption : public AmericanOption {
private:
    double _strike;
public:
    AmericanCallOption(double expiry, double strike)
        : AmericanOption(expiry), _strike(strike) {
    }

    double payoff(double spot) const override {
        return std::max(spot - _strike, 0.0);
    }
};
