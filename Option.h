#ifndef OPTION_H
#define OPTION_H

#include <vector>

class Option {
protected:
    double expiry;

public:
    Option(double expiry);
    virtual ~Option() = default;
    
    double getExpiry() const;
    virtual double payoff(double spot) const = 0;
    virtual double payoffPath(const std::vector<double>& spot_path) const;
    virtual bool isAsianOption() const;
    virtual bool isAmericanOption() const;
};

#endif