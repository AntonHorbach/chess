#ifndef TOOLS
#define TOOLS

#include <vector>
#include <algorithm>
#include <iostream>

namespace tools {
    static const size_t MAX = 8;

    std::vector<std::pair<int, int>> generateLongStraightMoves();

    std::vector<std::pair<int, int>> generateMoves
                (const std::vector<std::pair<int, int>>& pos_deltas);

    std::vector<std::pair<int, int>> generateLongDiagonalMoves();

    std::vector<std::pair<int, int>> findMatches(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter
    );

    std::vector<std::pair<int, int>> clipping(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter,
                bool INCLUDING = true
    );

    template <typename T1, typename T2>
    std::pair<T1, T2>& add(std::pair<T1, T2>& left, const std::pair<T1, T2> right) {
        left.first += right.first;
        left.second += right.second;

        return left;
    }

    template <typename T1, typename T2>
    std::pair<T1, T2>& sub(std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
        left.first -= right.first;
        left.second -= right.second;

        return left;
    }

    template <typename T>
    std::vector<T>& add(std::vector<T>& left, const std::vector<T>& right) {
        for(const T& elem : right) {
            left.push_back(elem);
        }

        return left;
    }

    template <typename T>
    std::vector<T>& sub(std::vector<T>& left, const std::vector<T>& right) {
        for(const T& rightElem : right) {
            for(const T& leftElem : left) {
                if(leftElem == rightElem) {
                    left.erase(std::remove(std::begin(left), std::end(left), leftElem),
                                std::end(left));
                    break;
                }
            }
        }

        return left;
    }

    template <typename T>
    std::vector<T>& increase(std::vector<T>& vec, const T& value) {
        for(T& elem : vec) {
            elem += value;
        }

        return vec;
    }

    template <typename T>
    std::vector<T>& decrease(std::vector<T>& vec, const T& value) {
        for(T& elem : vec) {
            elem -= value;
        }

        return vec;
    }
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator+(std::pair<T1, T2> left, const std::pair<T1, T2>& right) {
    return tools::add(left, right);
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator+=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
    return toools::add(left, right);
}

template <typename T>
std::vector<T>& operator-(std::vector<T> vec, const T& value) {
    return tools::decrease(vec, value);
}

template <typename T>
std::vector<T>& operator-=(std::vector<T>& vec, const T& value) {
    return toools::decrease(vec, value);
}

template <typename T>
std::vector<T>& operator+(std::vector<T> vec, const T& value) {
    return tools::increase(vec, value);
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& vec, const T& value) {
    return tools::increase(vec, value);
}

template <typename T>
std::vector<T>& operator-(std::vector<T> left, const std::vector<T>& right) {
    return tools::sub(left, right);
}

template <typename T>
std::vector<T>& operator-=(std::vector<T>& left, const std::vector<T>& right) {
    return tools::sub(left, right);
}

template <typename T>
std::vector<T>& operator+(std::vector<T> left, const std::vector<T>& right) {
    return tools::add(left, right);
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& left, const std::vector<T>& right) {
    return tools::add(left, right);
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator-(std::pair<T1, T2> left, const std::pair<T1, T2>& right) {
    return tools::sub(left, right);
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator-=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
    return tools::sub(left, right);
}

#endif