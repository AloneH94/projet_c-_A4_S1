#pragma once
#include "EuropeanVanillaOption.h"
#include <algorithm>

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}
    double payoff(double spot) const override { return std::max(spot - _strike, 0.0); }
    OptionType getOptionType() const override { return OptionType::Call; }
};
