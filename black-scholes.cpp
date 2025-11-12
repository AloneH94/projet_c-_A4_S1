double strike;
double date;
double spot;
double rate;
double vol;
enum OptionType {
    Call, Put    
};

class Option {
private:
    double _expiry;

public:
    Option(double expiry) : _expiry(expiry) {}

    double getExpiry() const {
        return _expiry;
    }

    virtual double payoff(double spot) const = 0;

    virtual ~Option() = default;
};

#include <stdexcept>
#include <cmath>
#include <type_traits>

class EuropeanVanillaOption : public Option {
private:
    double _strike;

public:
    enum class OptionType { Call, Put };

    EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike) {
        if (expiry < 0 || strike < 0) {
            throw std::invalid_argument("Expiry and strike must be non-negative");
        }
    }

    virtual OptionType GetOptionType() const = 0;

    double getStrike() const {
        return _strike;
    }

    friend class BlackScholesPricer;
};

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    double payoff(double spot) const override {
        return (spot >= getStrike()) ? (spot - getStrike()) : 0.0;
    }

    OptionType GetOptionType() const override {
        return OptionType::Call;
    }
};

class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    double payoff(double spot) const override {
        return (getStrike() >= spot) ? (getStrike() - spot) : 0.0;
    }

    OptionType GetOptionType() const override {
        return OptionType::Put;
    }
};

/***************  NOUVELLES OPTIONS DIGITALES  ***************/
class EuropeanDigitalCall : public EuropeanVanillaOption {
public:
    EuropeanDigitalCall(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    // Cash-or-nothing: paie 1 si S_T >= K, 0 sinon
    double payoff(double spot) const override {
        return (spot >= getStrike()) ? 1.0 : 0.0;
    }

    OptionType GetOptionType() const override {
        return OptionType::Call;
    }
};

class EuropeanDigitalPut : public EuropeanVanillaOption {
public:
    EuropeanDigitalPut(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    // Cash-or-nothing: paie 1 si S_T < K, 0 sinon
    double payoff(double spot) const override {
        return (spot < getStrike()) ? 1.0 : 0.0;
    }

    OptionType GetOptionType() const override {
        return OptionType::Put;
    }
};
/*************************************************************/

class BlackScholesPricer {
private:
    EuropeanVanillaOption* _option;
    double _S;
    double _r;
    double _sigma;

    double norm_cdf(double x) const {
        return 0.5 * std::erfc(-x / std::sqrt(2));
    }

public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility)
        : _option(option), _S(asset_price), _r(interest_rate), _sigma(volatility) {}

    double operator()() const {
        double T = _option->getExpiry();
        double K = _option->_strike;
        if (T <= 0 || _sigma <= 0) {
            // Pour T=0 ou sigma=0, on renvoie la valeur intrinsèque
            return _option->payoff(_S);
        }
        double d1 = (std::log(_S / K) + ( _r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
        double d2 = d1 - _sigma * std::sqrt(T);

        // *** Détection digitale vs vanille ***
        if (dynamic_cast<EuropeanDigitalCall*>(_option) != nullptr) {
            // Prix digital call (cash-or-nothing) : e^{-rT} * N(d2)
            return std::exp(-_r * T) * norm_cdf(d2);
        } else if (dynamic_cast<EuropeanDigitalPut*>(_option) != nullptr) {
            // Prix digital put (cash-or-nothing) : e^{-rT} * N(-d2)
            return std::exp(-_r * T) * norm_cdf(-d2);
        }

        // *** Formules vanilles ***
        if (_option->GetOptionType() == EuropeanVanillaOption::OptionType::Call) {
            return _S * norm_cdf(d1) - K * std::exp(-_r * T) * norm_cdf(d2);
        } else {
            return K * std::exp(-_r * T) * norm_cdf(-d2) - _S * norm_cdf(-d1);
        }
    }

    // delta() conservé tel quel (non utilisé dans main)
    double delta() const {
        double T = _option->getExpiry();
        double K = _option->_strike;
        if (T <= 0 || _sigma <= 0) {
            if (_option->GetOptionType() == EuropeanVanillaOption::OptionType::Call) {
                return (_S > K) ? 1.0 : 0.0;
            } else {
                return (_S < K) ? -1.0 : 0.0;
            }
        }
        double d1 = (std::log(_S / K) + (_r + 0.5 * _sigma * _sigma) * T) / (_sigma * std::sqrt(T));
        if (_option->GetOptionType() == EuropeanVanillaOption::OptionType::Call) {
            return norm_cdf(d1);
        } else {
            return norm_cdf(d1) - 1.0;
        }
    }
};

#include <iostream>
int main() {
    // Parameters
    double S0 = 100.0;
    double K = 101.0;
    double T = 5;
    double r = 0.01;
    double sigma = 0.1;

    // Create Call and Put options (vanille)
    CallOption call(T, K);
    PutOption put(T, K);

    // Create Digital Call and Put
    EuropeanDigitalCall dcall(T, K);
    EuropeanDigitalPut dput(T, K);

    // Instantiate Black-Scholes pricers
    BlackScholesPricer call_pricer(&call, S0, r, sigma);
    BlackScholesPricer put_pricer(&put, S0, r, sigma);
    BlackScholesPricer dcall_pricer(&dcall, S0, r, sigma);
    BlackScholesPricer dput_pricer(&dput, S0, r, sigma);

    // Compute and print results for vanilla Call
    std::cout << "European Call Option:" << std::endl;
    std::cout << "  Price: " << call_pricer() << std::endl;

    // Compute and print results for vanilla Put
    std::cout << "European Put Option:" << std::endl;
    std::cout << "  Price: " << put_pricer() << std::endl;

    // Compute and print results for Digital Call
    std::cout << "European Digital Call Option:" << std::endl;
    std::cout << "  Price: " << dcall_pricer() << std::endl;

    // Compute and print results for Digital Put
    std::cout << "European Digital Put Option:" << std::endl;
    std::cout << "  Price: " << dput_pricer() << std::endl;

    return 0;
}

/*
To compile and execute this program using g++, use the following commands:

    g++ -std=c++11 -o black_scholes black-scholes.cpp
    ./black_scholes
*/
