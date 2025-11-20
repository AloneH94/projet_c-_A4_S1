#pragma once
#include "AsianOption.h"
#include <algorithm>

class AsianCallOption : public AsianOption {
private:
    double _strike;
public:
    AsianCallOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps), _strike(strike) {
    }

    double payoff(double S) const override {
        return std::max(S - _strike, 0.0);
    }
};
