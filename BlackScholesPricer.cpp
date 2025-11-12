#ifndef BLACKSCHOLESPRICER_H
#define BLACKSCHOLESPRICER_H

#include "EuropeanVanillaOption.h"

class BlackScholesPricer {
private:
    EuropeanVanillaOption* option;
    double asset_price;
    double interest_rate;
    double volatility;

    // Helper functions for Black-Scholes formula
    double norm_cdf(double x) const;
    double calculate_d1() const;
    double calculate_d2() const;

public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, 
                      double interest_rate, double volatility);
    
    double operator()() const;  // Price calculation
    double delta() const;       // Delta calculation
};

#endif