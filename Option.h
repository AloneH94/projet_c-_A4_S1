#pragma once
class Option {
protected:
    double _expiry;
public:
    Option(double expiry) : _expiry(expiry) {}
    virtual ~Option() = default;
    double getExpiry() const { return _expiry; }
    virtual double payoff(double spot) const = 0;
};
