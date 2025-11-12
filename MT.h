#pragma once
#include <random>

class MT {
private:
    std::mt19937 _mt;
    MT() : _mt(std::random_device{}()) {}
public:
    static MT& getInstance() {
        static MT instance;
        return instance;
    }

    double rand_unif() { return std::uniform_real_distribution<double>(0.0, 1.0)(_mt); }
    double rand_norm() { return std::normal_distribution<double>(0.0, 1.0)(_mt); }

    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;
};
