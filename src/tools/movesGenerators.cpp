#include "tools.h"

namespace tools {
std::vector<std::pair<int, int>> generateLongStraightMoves(const size_t MAX)
{
    std::vector<std::pair<int, int>> res;

    for(size_t i = 1; i < MAX; ++i) {
        res.push_back({0, i});
        res.push_back({i, 0});
        res.push_back({0, -i});
        res.push_back({-i, 0});
    }

    return res;
}

std::vector<std::pair<int, int>> generateLongDiagonalMoves(const size_t MAX)
{
    std::vector<std::pair<int, int>> res;

    for(size_t i = 1; i < MAX; ++i) {
        res.push_back({i, i});
        res.push_back({-i, -i});
        res.push_back({-i, i});
        res.push_back({i, -i});
    }

    return res;
}

std::vector<std::pair<int, int>> generateMoves(const std::vector<std::pair<int, int>>& pos_deltas)
{
    std::vector<std::pair<int, int>> res;

    for(size_t i = 0; i < pos_deltas.size(); ++i) {
        res.push_back({pos_deltas[i].first, pos_deltas[i].second});
        res.push_back({-pos_deltas[i].first, pos_deltas[i].second});
        res.push_back({pos_deltas[i].first, -pos_deltas[i].second});
        res.push_back({-pos_deltas[i].first, -pos_deltas[i].second});
    }

    return res;
}
}