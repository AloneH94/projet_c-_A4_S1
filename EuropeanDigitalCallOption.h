#pragma once
#include "EuropeanVanillaOption.h"

class EuropeanDigitalCallOption : public EuropeanVanillaOption {
public:
    EuropeanDigitalCallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}
    double payoff(double spot) const override { return (spot >= _strike) ? 1.0 : 0.0; }
    OptionType getOptionType() const override { return OptionType::Call; }
};
