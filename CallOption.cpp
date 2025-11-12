#include "CallOption.h"
#include <algorithm>

CallOption::CallOption(double expiry, double strike) 
    : EuropeanVanillaOption(expiry, strike) {}

double CallOption::payoff(double spot) const {
    return std::max(spot - getStrike(), 0.0);
}

EuropeanVanillaOption::OptionType CallOption::GetOptionType() const {
    return EuropeanVanillaOption::OptionType::CALL;
}