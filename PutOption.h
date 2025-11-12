#pragma once
#include "Option.h"
#include <algorithm>

class PutOption : public Option {
    private:
        double _strike;
    public:
        PutOption(double expiry, double strike) : Option(expiry), _strike(strike) {}
        double getStrike() const { return _strike; }
        double payoff(double spot) const override {
            return std::max(_strike - spot, 0.0);
        }
};
