#pragma once
#include <vector>
#include <iostream>

template<typename T>
class BinaryTree {
private:
    int _depth;
    std::vector<std::vector<T>> _tree;

public:
    void setDepth(int depth) {
        _depth = depth;
        _tree.resize(_depth + 1);
        for (int i = 0; i <= _depth; ++i)
            _tree[i].resize(i + 1);
    }

    void setNode(int level, int index, T value) { _tree[level][index] = value; }
    T getNode(int level, int index) const { return _tree[level][index]; }

    void display() const {
        for (int i = 0; i <= _depth; ++i) {
            for (int j = 0; j <= i; ++j)
                std::cout << _tree[i][j] << " ";
            std::cout << std::endl;
        }
    }
};
