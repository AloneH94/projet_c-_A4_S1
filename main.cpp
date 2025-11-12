#include <iostream>
#include <iomanip>

#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "AmericanCallOption.h"
#include "AmericanPutOption.h"

#include "CRRPricer.h"
#include "BlackScholesPricer.h"

int main() {
    std::cout << std::fixed << std::setprecision(6);

    // === Paramètres communs ===
    double S0 = 100.0;      // prix initial de l'actif
    double K = 101.0;       // strike
    double T = 5.0;         // maturité (en années)
    double R = 0.01;        // taux sans risque (CRR)
    double U = 0.05;        // facteur de hausse
    double D = -0.045;       // facteur de baisse
    int N = 5;            // profondeur de l'arbre (pour précision)

    double r = 0.01;        // taux sans risque (Black-Scholes)
    double sigma = 0.1;     // volatilité (Black-Scholes)

    // === Options Européennes ===
    CallOption euroCall(T, K);
    PutOption euroPut(T, K);
    EuropeanDigitalCallOption digiCall(T, K);
    EuropeanDigitalPutOption digiPut(T, K);

    // === Options Américaines ===
    AmericanCallOption amerCall(T, K);
    AmericanPutOption amerPut(T, K);

    // === Pricers CRR ===
    CRRPricer crrEuroCall(&euroCall, N, S0, U, D, R);
    CRRPricer crrEuroPut(&euroPut, N, S0, U, D, R);
    CRRPricer crrDigiCall(&digiCall, N, S0, U, D, R);
    CRRPricer crrDigiPut(&digiPut, N, S0, U, D, R);
    CRRPricer crrAmerCall(&amerCall, N, S0, U, D, R);
    CRRPricer crrAmerPut(&amerPut, N, S0, U, D, R);

    // === Pricers Black-Scholes ===
    BlackScholesPricer bsEuroCall(&euroCall, S0, r, sigma, K);
    BlackScholesPricer bsEuroPut(&euroPut, S0, r, sigma, K);
    BlackScholesPricer bsDigiCall(&digiCall, S0, r, sigma, K);
    BlackScholesPricer bsDigiPut(&digiPut, S0, r, sigma, K);

    // === Calculs ===
    std::cout << "===== Cox-Ross-Rubinstein (CRR) =====\n";
    std::cout << "European Call:  " << crrEuroCall() << "\n";
    std::cout << "European Put:   " << crrEuroPut() << "\n";
    std::cout << "Digital Call:   " << crrDigiCall() << "\n";
    std::cout << "Digital Put:    " << crrDigiPut() << "\n";
    std::cout << "American Call:  " << crrAmerCall() << "\n";
    std::cout << "American Put:   " << crrAmerPut() << "\n";

    std::cout << "\n===== Black–Scholes =====\n";
    std::cout << "European Call:  " << bsEuroCall() << "\n";
    std::cout << "European Put:   " << bsEuroPut() << "\n";
    std::cout << "Digital Call:   " << bsDigiCall() << "\n";
    std::cout << "Digital Put:    " << bsDigiPut() << "\n";

    return 0;
}
