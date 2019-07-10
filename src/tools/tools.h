#ifndef TOOLS
#define TOOLS

#include <vector>
#include <algorithm>
#include <iostream>

template <typename T1, typename T2>
std::pair<T1, T2>& operator+=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right);

template <typename T1, typename T2>
std::pair<T1, T2>& operator+(std::pair<T1, T2> left, const std::pair<T1, T2>& right);

template <typename T>
std::vector<T>& operator-=(std::vector<T>& vec, const T& value);

template <typename T>
std::vector<T>& operator-(std::vector<T> vec, const T& value);

template <typename T>
std::vector<T>& operator+=(std::vector<T>& vec, const T& value);

template <typename T>
std::vector<T>& operator+(std::vector<T> vec, const T& value);

template <typename T>
std::vector<T>& operator-=(std::vector<T>& left, const std::vector<T>& right);

template <typename T>
std::vector<T>& operator-(std::vector<T> left, const std::vector<T>& right);

template <typename T>
std::vector<T>& operator+=(std::vector<T>& left, const std::vector<T>& right);

template <typename T>
std::vector<T>& operator+(std::vector<T> left, const std::vector<T>& right);

template <typename T1, typename T2>
std::pair<T1, T2>& operator-=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right);

template <typename T1, typename T2>
std::pair<T1, T2>& operator-(std::pair<T1, T2> left, const std::pair<T1, T2>& right);

namespace tools {
    std::vector<std::pair<int, int>> generateLongStraightMoves(const size_t MAX = 8);

    std::vector<std::pair<int, int>> generateMoves
                (const std::vector<std::pair<int, int>>& pos_deltas);

    std::vector<std::pair<int, int>> generateLongDiagonalMoves(const size_t MAX = 8);

    std::vector<std::pair<int, int>> findMatches(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter
    );

    std::vector<std::pair<int, int>> clipping(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter,
                bool INCLUDING = true, const size_t MAX = 8
    );

    std::vector<std::pair<int, int>> findMatches(
                int x1, int y1,
                int x2, int y2,
                const std::vector<std::pair<int, int>>& source
    );

    template <typename T>
    bool less_than(const std::initializer_list<T>& values, const T& value_to_compare);
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator+=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
    left.first += right.first;
    left.second += right.second;

    return left;
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator+(std::pair<T1, T2> left, const std::pair<T1, T2>& right) {
    return left += right;
}

template <typename T>
std::vector<T>& operator-=(std::vector<T>& vec, const T& value) {
    for(T& elem : vec) {
        elem -= value;
    }

    return vec;
}

template <typename T>
std::vector<T>& operator-(std::vector<T> vec, const T& value) {
    return vec -= value;
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& vec, const T& value) {
    for(T& elem : vec) {
        elem += value;
    }

    return vec;
}

template <typename T>
std::vector<T>& operator+(std::vector<T> vec, const T& value) {
    return vec += value;
}

template <typename T>
std::vector<T>& operator-=(std::vector<T>& left, const std::vector<T>& right) {
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
std::vector<T>& operator-(std::vector<T> left, const std::vector<T>& right) {
    return left -= right;
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& left, const std::vector<T>& right) {
    for(const T& elem : right) {
        left.push_back(elem);
    }

    return left;
}

template <typename T>
std::vector<T>& operator+(std::vector<T> left, const std::vector<T>& right) {
    return left += right;
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator-=(std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
    left.first -= right.first;
    left.second -= right.second;

    return left;
}

template <typename T1, typename T2>
std::pair<T1, T2>& operator-(std::pair<T1, T2> left, const std::pair<T1, T2>& right) {
    return left -= right;
}

namespace tools {
    template <typename T>
    bool less_than(const std::initializer_list<T>& values, const T& value_to_compare) {
        for(const T& value : values) {
            if(value >= value_to_compare) return false;
        }

        return true;
    }
}

#endif