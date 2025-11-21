#pragma once
#include "Option.h"

class EuropeanVanillaOption : public Option {
protected:
    double _strike;

public:
    enum class OptionType { Call, Put };

    EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike) {
        if (strike < 0.0) throw std::invalid_argument("Strike must be non-negative");
    }

    virtual OptionType getOptionType() const = 0;
    double getStrike() const { return _strike; }

    friend class BlackScholesPricer;
    friend class CRRPricer;
};
