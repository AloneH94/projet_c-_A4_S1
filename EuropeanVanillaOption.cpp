#ifndef EUROPEANVANILLAOPTION_H
#define EUROPEANVANILLAOPTION_H

#include "Option.h"

class BlackScholesPricer;  // Forward declaration

class EuropeanVanillaOption : public Option {
public:
    enum class OptionType { CALL, PUT };

private:
    double strike;

public:
    EuropeanVanillaOption(double expiry, double strike);
    double getStrike() const;
    virtual OptionType GetOptionType() const = 0;
    
    // Déclaration d'amitié pour permettre l'accès au strike
    friend class BlackScholesPricer;
};

#endif