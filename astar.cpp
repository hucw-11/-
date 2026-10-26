#include "astar.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <vector>

namespace {
struct Node {
    Board board;
    int g; // 从初始状态到当前状态的已知最小步数。
    int h; // 当前状态到目标状态的曼哈顿距离。
};

struct CompareNode {
    bool operator()(const Node& left, const Node& right) const {
        const int leftF = left.g + left.h;
        const int rightF = right.g + right.h;
        if (leftF != rightF) {
            return leftF > rightF; // priority_queue 中 f 较小者优先。
        }
        if (left.h != right.h) {
            return left.h > right.h;
        }
        return left.board > right.board; // 固定平局顺序，便于复现。
    }
};

struct ParentInfo {
    Board parent;
    Move move;
};

SearchResult reconstructPath(const Board& start, const Board& goal,
                             const std::map<Board, ParentInfo>& parents) {
    SearchResult result;
    result.solved = true;
    Board current = goal;
    result.path.push_back(current);
    while (current != start) {
        const ParentInfo& info = parents.at(current);
        result.moves.push_back(info.move);
        current = info.parent;
        result.path.push_back(current);
    }
    std::reverse(result.path.begin(), result.path.end());
    std::reverse(result.moves.begin(), result.moves.end());
    return result;
}
} // namespace

SearchResult solveAStar(const Board& start, const Board& goal) {
    if (!isValidBoard(start) || !isValidBoard(goal)) {
        throw std::invalid_argument("Each board must contain 0 through 15 exactly once.");
    }
    if (!isSolvable(start, goal)) {
        return {};
    }

    std::priority_queue<Node, std::vector<Node>, CompareNode> open;
    std::set<Board> closed;
    std::map<Board, int> gScore;
    std::map<Board, ParentInfo> parents;

    gScore[start] = 0;
    open.push({start, 0, manhattanDistance(start, goal)});

    while (!open.empty()) {
        const Node current = open.top();
        open.pop();

        // 更新代价时直接加入新记录，出队时跳过旧记录。
        if (current.g != gScore.at(current.board)
            || closed.count(current.board) != 0) {
            continue;
        }
        if (current.board == goal) {
            return reconstructPath(start, goal, parents);
        }
        closed.insert(current.board);

        for (const Neighbor& next : getNeighbors(current.board)) {
            // 曼哈顿距离是一致启发函数，已扩展状态无需重新打开。
            if (closed.count(next.board) != 0) {
                continue;
            }
            const int newG = current.g + 1;
            const auto found = gScore.find(next.board);
            if (found == gScore.end() || newG < found->second) {
                gScore[next.board] = newG;
                parents.insert_or_assign(next.board, ParentInfo{current.board, next.move});
                open.push({next.board, newG, manhattanDistance(next.board, goal)});
            }
        }
    }
    return {};
}
