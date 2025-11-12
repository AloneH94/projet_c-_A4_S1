#ifndef CALLOPTION_H
#define CALLOPTION_H

#include "EuropeanVanillaOption.h"

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike);
    double payoff(double spot) const override;
    EuropeanVanillaOption::OptionType GetOptionType() const override;
};

#endif