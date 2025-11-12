#pragma once
#include "Option.h"

class EuropeanDigitalPutOption : public Option {
    private:
        double _strike;
    public:
        EuropeanDigitalPutOption(double expiry, double strike)
            : Option(expiry), _strike(strike) {}
        double payoff(double spot) const override { return spot <= _strike ? 1.0 : 0.0; }
    };