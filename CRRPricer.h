#pragma once
#include <cmath>
#include <stdexcept>
#include "Option.h"
#include "BinaryTree.h"

class CRRPricer {
private:
    Option* _option;
    int _depth;
    double _S0, _U, _D, _R;
    BinaryTree<double> _priceTree, _optionTree;
    bool _computed = false;

public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate)
        : _option(option), _depth(depth), _S0(asset_price), _U(up), _D(down), _R(interest_rate) {
        // Convert U and D to multiplicative factors if they look like percentages
        if (_U < 0.5 && _U > -0.5) _U = 1.0 + _U;
        if (_D < 0.5 && _D > -0.5) _D = 1.0 + _D;
        // Check no-arbitrage condition
        if (_D >= (1.0 + _R) || (1.0 + _R) >= _U)
            throw std::invalid_argument("Arbitrage condition violated");
        _priceTree.setDepth(_depth);
        _optionTree.setDepth(_depth);
    }

    void compute() {
        // Build the price tree
        for (int n = 0; n <= _depth; ++n)
            for (int i = 0; i <= n; ++i)
                _priceTree.setNode(n, i, _S0 * pow(_U, i) * pow(_D, n - i));

        // Compute option payoff at maturity
        for (int i = 0; i <= _depth; ++i)
            _optionTree.setNode(_depth, i, _option->payoff(_priceTree.getNode(_depth, i)));

        double p = ((1.0 + _R) - _D) / (_U - _D);
        if (p < 0.0 || p > 1.0)
            throw std::invalid_argument("Risk-neutral probability out of bounds");

        // Precompute discount factor
        double discount = 1.0 / (1.0 + _R);

        // Detect if the option is American type
        bool isAmerican = (dynamic_cast<AmericanCallOption*>(_option) || dynamic_cast<AmericanPutOption*>(_option));

        // Backward induction
        for (int n = _depth - 1; n >= 0; --n)
            for (int i = 0; i <= n; ++i) {
                double Hup = _optionTree.getNode(n + 1, i + 1);
                double Hdown = _optionTree.getNode(n + 1, i);
                double exercise = _option->payoff(_priceTree.getNode(n, i));
                double value = discount * (p * Hup + (1 - p) * Hdown);
                if (isAmerican)
                    value = std::max(value, exercise);
                _optionTree.setNode(n, i, value);
            }

        _computed = true;
    }

    double operator()(bool closed_form = false) {
        if (closed_form) {
            double p = ((1.0 + _R) - _D) / (_U - _D);
            if (p < 0.0 || p > 1.0)
                throw std::invalid_argument("Risk-neutral probability out of bounds");
            double value = 0.0;
            for (int i = 0; i <= _depth; ++i) {
                double comb = std::tgamma(_depth + 1) / (std::tgamma(i + 1) * std::tgamma(_depth - i + 1));
                double prob = std::pow(p, i) * std::pow(1 - p, _depth - i);
                double spot = _S0 * std::pow(_U, i) * std::pow(_D, _depth - i);
                double payoff = _option->payoff(spot);
                value += comb * prob * payoff;
            }
            double discount = std::pow(1.0 / (1.0 + _R), _depth);
            return value * discount;
        }
        if (!_computed) compute();
        return _optionTree.getNode(0, 0);
    }
};
