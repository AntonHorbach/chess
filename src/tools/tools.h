#ifndef TOOLS
#define TOOLS

#include <vector>
#include <algorithm>
#include <iostream>

namespace tools {
    struct Memory {
        int x, y;
        int dx, dy;
    };

    static const size_t MAX = 8;

    inline bool pairComparator(const std::pair<int, int>& left,
                            const std::pair<int, int>& right)
    {
        if(left.first < right.first) {
            return true;
        }
        else if(left.first == right.first) {
            if(left.second < right.second) {
                return true;
            }
            else return false;
        }
        else return false;
    }

    std::vector<std::pair<int, int>> generateLongStraightMoves();

    std::vector<std::pair<int, int>> generateMoves
                (const std::vector<std::pair<int, int>>& pos_deltas);

    std::vector<std::pair<int, int>> generateLongDiagonalMoves();

    template <typename T>
    std::vector<T>& add(std::vector<T>& left, const std::vector<T>& right) {
        for(const auto& elem : right) {
            left.push_back(elem);
        }

        return left;
    }

    std::vector<std::pair<int, int>> findMatches(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter
    );

    std::vector<std::pair<int, int>> clipping(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter,
                bool INCLUDING = false
    );
}
#endif