#pragma once
#include "EuropeanVanillaOption.h"
#include "AmericanOption.h"
#include "BinaryTree.h"
#include <cmath>
#include <stdexcept>

class CRRPricer {
private:
    Option* _option;  // Peut être EuropeanVanillaOption ou AmericanOption
    int _depth;
    double _S0, _U, _D, _R;
    BinaryTree<double> _tree;
    BinaryTree<bool> _exercise; // Pour American
    bool _computed;

    // Calcul du prix de l'actif au n-ième pas et i-ème noeud
    double stockPrice(int n, int i) const {
        return _S0 * std::pow(1 + _U, i) * std::pow(1 + _D, n - i);
    }

public:
    // Constructeur standard (U, D, R fournis)
    CRRPricer(Option* option, int depth, double S0, double U, double D, double R)
        : _option(option), _depth(depth), _S0(S0), _U(U), _D(D), _R(R), _computed(false) {

        // Vérification option asiatique
        if (_option->isAsianOption())
            throw std::invalid_argument("CRR not compatible with Asian options");

        if (!(_D < _R && _R < _U))
            throw std::invalid_argument("Arbitrage condition violated: D < R < U");

        _tree.setDepth(depth);
        if (_option->isAmericanOption()) _exercise.setDepth(depth);
    }

    // Constructeur avec approximation Black-Scholes (r, sigma)
    CRRPricer(Option* option, int depth, double S0, double r, double sigma)
        : _option(option), _depth(depth), _S0(S0), _computed(false) {

        // Vérification option asiatique
        if (_option->isAsianOption())
            throw std::invalid_argument("CRR not compatible with Asian options");

        double h = option->getExpiry() / depth;
        _U = std::exp((r + 0.5 * sigma * sigma) * h + sigma * std::sqrt(h)) - 1.0;
        _D = std::exp((r + 0.5 * sigma * sigma) * h - sigma * std::sqrt(h)) - 1.0;
        _R = std::exp(r * h) - 1.0;

        if (!(_D < _R && _R < _U))
            throw std::invalid_argument("Arbitrage condition violated: D < R < U");

        _tree.setDepth(depth);
        if (_option->isAmericanOption()) _exercise.setDepth(depth);
    }

    // Calcul du prix CRR
    void compute() {
        double q = (_R - _D) / (_U - _D);

        // Valeurs à l'échéance
        for (int i = 0; i <= _depth; ++i)
            _tree.setNode(_depth, i, _option->payoff(stockPrice(_depth, i)));

        // Induction arrière
        for (int n = _depth - 1; n >= 0; --n)
            for (int i = 0; i <= n; ++i) {
                double cont = (q * _tree.getNode(n + 1, i + 1) + (1 - q) * _tree.getNode(n + 1, i)) / (1 + _R);
                double intrinsic = _option->payoff(stockPrice(n, i));
                double H = cont;
                if (_option->isAmericanOption()) {
                    H = std::max(cont, intrinsic);
                    _exercise.setNode(n, i, intrinsic >= cont);
                }
                _tree.setNode(n, i, H);
            }

        _computed = true;
    }

    // Récupérer le prix d'un noeud
    double get(int n, int i) {
        if (!_computed) compute();
        return _tree.getNode(n, i);
    }

    // Récupérer la décision d'exercice (American)
    bool getExercise(int n, int i) {
        if (!_option->isAmericanOption())
            throw std::runtime_error("Option is not American");
        if (!_computed) compute();
        return _exercise.getNode(n, i);
    }

    // Prix CRR classique
    double operator()() {
        if (!_computed) compute();
        return _tree.getNode(0, 0);
    }

    // Prix CRR avec option closed-form (formule combinatoire)
    double operator()(bool closed_form) {
        if (closed_form) {
            if (_option->isAmericanOption())
                return operator()(); // pas de closed-form pour American

            double q = (_R - _D) / (_U - _D);
            double value = 0.0;
            for (int i = 0; i <= _depth; ++i) {
                double comb = std::tgamma(_depth + 1) / (std::tgamma(i + 1) * std::tgamma(_depth - i + 1));
                double prob = std::pow(q, i) * std::pow(1 - q, _depth - i);
                double spot = stockPrice(_depth, i);
                double payoff = _option->payoff(spot);
                value += comb * prob * payoff;
            }
            double discount = std::pow(1.0 / (1.0 + _R), _depth);
            return value * discount;
        }
        return operator()();
    }
};
