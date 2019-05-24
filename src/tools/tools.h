#ifndef TOOLS
#define TOOLS

#include <vector>

namespace tools {
    static const size_t MAX = 8;

    std::vector<std::pair<int, int>> generateLongStraightMoves();

    std::vector<std::pair<int, int>> generateMoves
                (const std::vector<std::pair<int, int>>& pos_deltas);

    std::vector<std::pair<int, int>> generateLongVerticalMoves();
}
#endif