#pragma once
#include <cmath>
#include <stdexcept>
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"

class BlackScholesPricer {
private:
    Option* _option;
    double _S, _r, _sigma, _T, _K;

    static double norm_cdf(double x) {
        return 0.5 * std::erfc(-x * M_SQRT1_2);
    }

public:
    BlackScholesPricer(Option* option, double S, double r, double sigma, double K)
        : _option(option), _S(S), _r(r), _sigma(sigma), _T(option->getExpiry()), _K(K) {}

    double operator()() const {
        double d1 = (std::log(_S / _K) + (_r + 0.5 * _sigma * _sigma) * _T) / (_sigma * std::sqrt(_T));
        double d2 = d1 - _sigma * std::sqrt(_T);

        // --- European Call ---
        if (dynamic_cast<CallOption*>(_option)) {
            return _S * norm_cdf(d1) - _K * std::exp(-_r * _T) * norm_cdf(d2);
        }

        // --- European Put ---
        if (dynamic_cast<PutOption*>(_option)) {
            return _K * std::exp(-_r * _T) * norm_cdf(-d2) - _S * norm_cdf(-d1);
        }

        // --- Digital Call ---
        if (dynamic_cast<EuropeanDigitalCallOption*>(_option)) {
            return std::exp(-_r * _T) * norm_cdf(d2);
        }

        // --- Digital Put ---
        if (dynamic_cast<EuropeanDigitalPutOption*>(_option)) {
            return std::exp(-_r * _T) * norm_cdf(-d2);
        }

        throw std::invalid_argument("Unsupported option type for Black–Scholes");
    }
};
