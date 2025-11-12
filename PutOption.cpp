#include "PutOption.h"
#include <algorithm>

PutOption::PutOption(double expiry, double strike) 
    : EuropeanVanillaOption(expiry, strike) {}

double PutOption::payoff(double spot) const {
    return std::max(getStrike() - spot, 0.0);
}

EuropeanVanillaOption::OptionType PutOption::GetOptionType() const {
    return EuropeanVanillaOption::OptionType::PUT;
}