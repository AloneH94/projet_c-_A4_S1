#pragma once
#include "EuropeanVanillaOption.h"
#include <algorithm>

class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}
    double payoff(double spot) const override { return std::max(_strike - spot, 0.0); }
    OptionType getOptionType() const override { return OptionType::Put; }
};
