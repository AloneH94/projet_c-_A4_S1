#pragma once
#include "Option.h"
#include <vector>
#include <numeric>
#include <stdexcept>

class AsianOption : public Option {
protected:
    std::vector<double> _timeSteps;

public:
    AsianOption(const std::vector<double>& timeSteps)
        : Option(timeSteps.back()), _timeSteps(timeSteps) {
        if (timeSteps.empty()) throw std::invalid_argument("timeSteps cannot be empty");
    }

    const std::vector<double>& getTimeSteps() const { return _timeSteps; }

    double payoffPath(const std::vector<double>& path) const override final {
        double sum = std::accumulate(path.begin(), path.end(), 0.0);
        double avg = sum / path.size();
        return payoff(avg);
    }

    bool isAsianOption() const override { return true; }
};
