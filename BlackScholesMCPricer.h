#pragma once
#include "Option.h"
#include "AsianOption.h"
#include "MT.h"
#include <vector>
#include <cmath>
#include <stdexcept>
#include <numeric>

class BlackScholesMCPricer {
private:
    Option* _option;
    double _S0, _r, _sigma;
    double _price;
    int _nbPaths;
    bool _computed;

public:
    BlackScholesMCPricer(Option* option, double S0, double r, double sigma)
        : _option(option), _S0(S0), _r(r), _sigma(sigma), _price(0.0), _nbPaths(0), _computed(false) {
    }

    void generate(int nb_paths) {
        MT& rng = MT::getInstance();
        int m = _option->isAsianOption() ? dynamic_cast<AsianOption*>(_option)->getTimeSteps().size() : 1;
        std::vector<double> path(m);
        double sumPayoff = 0.0;

        for (int i = 0; i < nb_paths; ++i) {
            if (m == 1) { // European / Digital
                double Z = rng.rand_norm();
                double ST = _S0 * std::exp((_r - 0.5 * _sigma * _sigma) * _option->getExpiry()
                    + _sigma * std::sqrt(_option->getExpiry()) * Z);
                sumPayoff += _option->payoff(ST);
            }
            else { // Asian
                AsianOption* asianOpt = dynamic_cast<AsianOption*>(_option);
                const auto& times = asianOpt->getTimeSteps();
                path[0] = _S0 * std::exp((_r - 0.5 * _sigma * _sigma) * times[0] + _sigma * std::sqrt(times[0]) * rng.rand_norm());
                for (size_t k = 1; k < times.size(); ++k) {
                    double dt = times[k] - times[k - 1];
                    path[k] = path[k - 1] * std::exp((_r - 0.5 * _sigma * _sigma) * dt + _sigma * std::sqrt(dt) * rng.rand_norm());
                }
                sumPayoff += asianOpt->payoffPath(path);
            }
        }

        _price = (_price * _nbPaths + sumPayoff) / (_nbPaths + nb_paths);
        _nbPaths += nb_paths;
        _computed = true;
    }

    double operator()() const {
        if (!_computed) throw std::runtime_error("Price undefined, generate paths first");
        return std::exp(-_r * _option->getExpiry()) * _price;
    }

    int getNbPaths() const { return _nbPaths; }

    std::vector<double> confidenceInterval() const {
        if (!_computed) throw std::runtime_error("Price undefined, generate paths first");
        double alpha = 1.96; // 95% CI
        double se = std::sqrt(_price * (1 - _price) / std::max(1, _nbPaths));
        double discount = std::exp(-_r * _option->getExpiry());
        return { _price * discount - alpha * se, _price * discount + alpha * se };
    }
};
