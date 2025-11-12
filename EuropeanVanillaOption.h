#ifndef EUROPEANVANILLAOPTION_H
#define EUROPEANVANILLAOPTION_H

#include "Option.h"

class BlackScholesPricer;  // Forward declaration

class EuropeanVanillaOption : public Option {
public:
    enum class OptionType { CALL, PUT };

private:
    double strike;
    friend class BlackScholesPricer;  // BlackScholesPricer can access strike

public:
    EuropeanVanillaOption(double expiry, double strike);
    virtual OptionType GetOptionType() const = 0;
    double getStrike() const;
};

#endif