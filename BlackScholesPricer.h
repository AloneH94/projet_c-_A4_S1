#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include <cmath>

class BlackScholesPricer {
private:
    EuropeanVanillaOption* _option;
    double _S, _r, _sigma;

    double norm_cdf(double x) const {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price,
        double interest_rate, double volatility)
        : _option(option), _S(asset_price), _r(interest_rate), _sigma(volatility) {
    }

    double operator()() const {
        double K = _option->_strike;
        double T = _option->getExpiry();

        // Cas option digitale
        if (dynamic_cast<EuropeanDigitalCallOption*>(_option)) {
            double d2 = (std::log(_S / K) + (_r - 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
            return std::exp(-_r * T) * norm_cdf(d2);
        }
        if (dynamic_cast<EuropeanDigitalPutOption*>(_option)) {
            double d2 = (std::log(_S / K) + (_r - 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
            return std::exp(-_r * T) * norm_cdf(-d2);
        }

        // Cas option vanille
        double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
        double d2 = d1 - _sigma * std::sqrt(T);

        if (_option->getOptionType() == EuropeanVanillaOption::OptionType::Call)
            return _S * norm_cdf(d1) - K * std::exp(-_r * T) * norm_cdf(d2);
        else
            return K * std::exp(-_r * T) * norm_cdf(-d2) - _S * norm_cdf(-d1);
    }

    double delta() const {
        double K = _option->_strike;
        double T = _option->getExpiry();

        // Cas option digitale
        if (dynamic_cast<EuropeanDigitalCallOption*>(_option)) {
            double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
            double d2 = d1 - _sigma * std::sqrt(T);
            return norm_cdf(d2); // delta approximation digitale
        }
        if (dynamic_cast<EuropeanDigitalPutOption*>(_option)) {
            double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
            double d2 = d1 - _sigma * std::sqrt(T);
            return norm_cdf(d2) - 1.0; // delta approximation digitale
        }

        // Cas option vanille
        double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
        if (_option->getOptionType() == EuropeanVanillaOption::OptionType::Call)
            return norm_cdf(d1);
        else
            return norm_cdf(d1) - 1.0;
    }
};
