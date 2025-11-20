#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class BinaryTree {
private:
    int _depth;
    std::vector<std::vector<T>> _tree;

public:
    BinaryTree() : _depth(0) {}

    void setDepth(int depth) {
        if (depth < 0) throw std::invalid_argument("Depth must be non-negative");
        _depth = depth;
        _tree.resize(_depth + 1);
        for (int i = 0; i <= _depth; ++i)
            _tree[i].resize(i + 1);
    }

    void setNode(int n, int i, const T& value) {
        if (n < 0 || n > _depth || i < 0 || i > n) throw std::out_of_range("Invalid node indices");
        _tree[n][i] = value;
    }

    T getNode(int n, int i) const {
        if (n < 0 || n > _depth || i < 0 || i > n) throw std::out_of_range("Invalid node indices");
        return _tree[n][i];
    }

    void display() const {
        for (int n = 0; n <= _depth; ++n) {
            for (int i = 0; i <= n; ++i)
                std::cout << std::fixed << std::setprecision(4) << _tree[n][i] << " ";
            std::cout << std::endl;
        }
    }

    int depth() const { return _depth; }
};
