#include "Option.h"
#include <stdexcept>
#include <algorithm>

Option::Option(double expiry) : expiry(expiry) {
    if (expiry < 0) {
        throw std::invalid_argument("Expiry cannot be negative");
    }
}

double Option::getExpiry() const {
    return expiry;
}

double Option::payoffPath(const std::vector<double>& spot_path) const {
    if (spot_path.empty()) {
        return 0.0;
    }
    return payoff(spot_path.back());
}

bool Option::isAsianOption() const {
    return false;
}

bool Option::isAmericanOption() const {
    return false;
}