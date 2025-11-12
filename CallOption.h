#pragma once
#include "Option.h"
#include <algorithm>

class CallOption : public Option {
private:
    double _strike;
public:
    CallOption(double expiry, double strike) : Option(expiry), _strike(strike) {}
    double getStrike() const { return _strike; }
    double payoff(double spot) const override {
        return std::max(spot - _strike, 0.0);
    }
};
