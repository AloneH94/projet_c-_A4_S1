#pragma once
#include "Option.h"

class EuropeanDigitalCallOption : public Option {
private:
    double _strike;
public:
    EuropeanDigitalCallOption(double expiry, double strike)
        : Option(expiry), _strike(strike) {}
    double payoff(double spot) const override { return spot >= _strike ? 1.0 : 0.0; }
};