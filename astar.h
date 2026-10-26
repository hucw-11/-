#ifndef ASTAR_H
#define ASTAR_H

#include "puzzle.h"

struct SearchResult {
    bool solved = false;
    std::vector<Board> path; // 含初始状态和目标状态。
    std::vector<Move> moves; // moves[i] 将 path[i] 变为 path[i + 1]。
};

// 非法棋盘抛出 std::invalid_argument；不可解时返回 solved == false。
SearchResult solveAStar(const Board& start, const Board& goal);

#endif
