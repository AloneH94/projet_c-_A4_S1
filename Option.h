#pragma once
#include <vector>
#include <stdexcept>

class Option {
protected:
    double _expiry;

public:
    Option(double expiry) : _expiry(expiry) {
        if (expiry < 0.0) throw std::invalid_argument("Expiry must be non-negative");
    }

    virtual ~Option() = default;
    double getExpiry() const { return _expiry; }

    virtual double payoff(double spot) const = 0;

    virtual double payoffPath(const std::vector<double>& path) const {
        return payoff(path.back());
    }

    virtual bool isAsianOption() const { return false; }
    virtual bool isAmericanOption() const { return false; }
};
