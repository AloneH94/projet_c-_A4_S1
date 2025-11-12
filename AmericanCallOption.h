#pragma once
#include "Option.h"
#include <algorithm>

class AmericanCallOption : public Option {
private:
    double _strike;

public:
    AmericanCallOption(double expiry, double strike)
        : Option(expiry), _strike(strike) {
        if (expiry < 0.0) throw std::invalid_argument("Expiry must be non-negative");
        if (strike < 0.0) throw std::invalid_argument("Strike must be non-negative");
    }

    double getStrike() const { return _strike; }

    double payoff(double spot) const override {
        return std::max(spot - _strike, 0.0);
    }
};