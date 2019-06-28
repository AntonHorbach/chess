#include "tools.h"

namespace tools {
std::vector<std::pair<int, int>> clipping(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter,
                bool INCLUDING, const size_t MAX)
{
    struct Memory {
        int x, y;
        int dx, dy;
    };

    std::vector<std::pair<int, int>> res;
    std::vector<Memory> memory;

    for(size_t i = 0; i < source.size(); ++i) {
        bool flag = false;
        for(size_t j = 0; j < filter.size(); ++j) {
            if(x+source[i].first == filter[j].first && y+source[i].second == filter[j].second)
            {
                Memory mem;
                mem.x = filter[j].first;
                mem.y = filter[j].second;
                mem.dx = source[i].first;
                mem.dy = source[i].second;

                memory.push_back(mem);

                if(INCLUDING) flag = true;
                break;
            }
        }

        if(!flag) {
            res.push_back(source[i]);
        }
    }

    for(int i = 0; i < res.size(); ++i) {
        if(x + res[i].first < 0 || x + res[i].first >= MAX ||
            y + res[i].second < 0 || y + res[i].second >= MAX)
        {
            res.erase(std::remove(std::begin(res), std::end(res), res[i]), std::end(res));
            i -= 1;
            continue;
        }

        for(size_t j = 0; j < memory.size(); ++j) {
            int dx  = memory[j].dx > 0 ? 1 : (memory[j].dx < 0 ? -1 : 0);
            int dy  = memory[j].dy > 0 ? 1 : (memory[j].dy < 0 ? -1 : 0);
            int count = std::max(abs(memory[j].x - res[i].first), abs(memory[j].y - res[i].second));

            for(size_t k = 1; k <= count + 1; ++k) {
                if(memory[j].x + dx * k == x + res[i].first &&
                     memory[j].y + dy * k == y + res[i].second)
                {
                    res.erase(std::remove(std::begin(res), std::end(res), res[i]), std::end(res));
                    i = -1;
                    break;
                }
            }

            if(i == -1) break;
        }
    }

    return res;
}

std::vector<std::pair<int, int>> findMatches(
                int x, int y,
                const std::vector<std::pair<int, int>>& source,
                const std::vector<std::pair<int, int>>& filter)
{
    std::vector<std::pair<int, int>> res;

    for(size_t i = 0; i < source.size(); ++i) {
        if(std::find(std::begin(filter), std::end(filter),
                    source[i] + std::pair<int, int>{x, y})
            != std::end(filter))
        {
            res.push_back(source[i]);
        }
    }

    return res;
}

std::vector<std::pair<int, int>> findMatches(
                int x1, int y1,
                int x2, int y2,
                const std::vector<std::pair<int, int>>& source)
{
    std::vector<std::pair<int, int>> res;
    std::vector<std::pair<int, int>> range;

    int dx  = abs(x1 - x2) > 0 ? 1 : (abs(x1 - x2) < 0 ? -1 : 0);
    int dy  = abs(y1 - y2) > 0 ? 1 : (abs(y1 - y2) < 0 ? -1 : 0);
    int x_ = x1, y_ = y1;

    while(x_ < x2 || y_ < y2) {
        range.push_back(std::pair<int, int>{x_ += dx, y_ += dy});
    }

    return findMatches(x1, y1, range, source);
}

}