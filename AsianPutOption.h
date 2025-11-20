#pragma once
#include "AsianOption.h"
#include <algorithm>

class AsianPutOption : public AsianOption {
private:
    double _strike;
public:
    AsianPutOption(const std::vector<double>& timeSteps, double strike)
        : AsianOption(timeSteps), _strike(strike) {
    }

    double payoff(double S) const override {
        return std::max(_strike - S, 0.0);
    }
};
