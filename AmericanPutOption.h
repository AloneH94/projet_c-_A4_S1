#pragma once
#include "AmericanOption.h"
#include <algorithm>

class AmericanPutOption : public AmericanOption {
private:
    double _strike;
public:
    AmericanPutOption(double expiry, double strike)
        : AmericanOption(expiry), _strike(strike) {
    }

    double payoff(double spot) const override {
        return std::max(_strike - spot, 0.0);
    }
};
