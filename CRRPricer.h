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
        if (_D >= (1.0 + _R) || (1.0 + _R) >= _U)
            throw std::invalid_argument("Arbitrage condition violated");
        _priceTree.setDepth(_depth);
        _optionTree.setDepth(_depth);
    }

    void compute() {
        for (int n = 0; n <= _depth; ++n)
            for (int i = 0; i <= n; ++i)
                _priceTree.setNode(n, i, _S0 * pow(_U, i) * pow(_D, n - i));

        for (int i = 0; i <= _depth; ++i)
            _optionTree.setNode(_depth, i, _option->payoff(_priceTree.getNode(_depth, i)));

        double p = ((1.0 + _R) - _D) / (_U - _D);
        for (int n = _depth - 1; n >= 0; --n)
            for (int i = 0; i <= n; ++i) {
                double Hup = _optionTree.getNode(n + 1, i + 1);
                double Hdown = _optionTree.getNode(n + 1, i);
                double value = (p * Hup + (1 - p) * Hdown) / (1.0 + _R);
                _optionTree.setNode(n, i, value);
            }

        _computed = true;
    }

    double operator()(bool closed_form = false) {
        if (!_computed) compute();
        return _optionTree.getNode(0, 0);
    }
};
