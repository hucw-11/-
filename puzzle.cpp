#include "puzzle.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <utility>

namespace {
// 4 x 4 棋盘中，该奇偶性在合法移动前后保持不变。
int parity(const Board& board) {
    int inversions = 0;
    int blankRowFromBottom = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == 0) {
            blankRowFromBottom = BOARD_WIDTH - i / BOARD_WIDTH;
            continue;
        }
        for (int j = i + 1; j < BOARD_SIZE; ++j) {
            if (board[j] != 0 && board[i] > board[j]) {
                ++inversions;
            }
        }
    }
    return (inversions + blankRowFromBottom) % 2;
}
} // namespace

bool isValidBoard(const Board& board) {
    std::array<bool, BOARD_SIZE> seen{};
    for (int value : board) {
        if (value < 0 || value >= BOARD_SIZE || seen[value]) {
            return false;
        }
        seen[value] = true;
    }
    return true;
}

bool isSolvable(const Board& start, const Board& goal) {
    return isValidBoard(start) && isValidBoard(goal)
        && parity(start) == parity(goal);
}

int manhattanDistance(const Board& board, const Board& goal) {
    std::array<int, BOARD_SIZE> goalPositions{};
    for (int i = 0; i < BOARD_SIZE; ++i) {
        goalPositions[goal[i]] = i;
    }

    int distance = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == 0) {
            continue; // 空格不计入启发函数。
        }
        const int target = goalPositions[board[i]];
        distance += std::abs(i / BOARD_WIDTH - target / BOARD_WIDTH)
                  + std::abs(i % BOARD_WIDTH - target % BOARD_WIDTH);
    }
    return distance;
}

std::vector<Neighbor> getNeighbors(const Board& board) {
    const auto blank = std::find(board.begin(), board.end(), 0);
    if (blank == board.end()) {
        return {};
    }
    const int index = static_cast<int>(blank - board.begin());
    const int row = index / BOARD_WIDTH;
    const int col = index % BOARD_WIDTH;

    const int rowChanges[] = {-1, 1, 0, 0};
    const int colChanges[] = {0, 0, -1, 1};
    const Move moves[] = {Move::Up, Move::Down, Move::Left, Move::Right};

    std::vector<Neighbor> neighbors;
    for (int i = 0; i < 4; ++i) {
        const int nextRow = row + rowChanges[i];
        const int nextCol = col + colChanges[i];
        if (nextRow < 0 || nextRow >= BOARD_WIDTH
            || nextCol < 0 || nextCol >= BOARD_WIDTH) {
            continue;
        }
        Board next = board;
        std::swap(next[index], next[nextRow * BOARD_WIDTH + nextCol]);
        neighbors.push_back({next, moves[i]});
    }
    return neighbors;
}

const char* moveName(Move move) {
    switch (move) {
    case Move::Up: return "Up";
    case Move::Down: return "Down";
    case Move::Left: return "Left";
    case Move::Right: return "Right";
    }
    return "Unknown";
}

void printBoard(const Board& board, std::ostream& out) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == 0) {
            out << std::setw(3) << '.';
        } else {
            out << std::setw(3) << board[i];
        }
        if ((i + 1) % BOARD_WIDTH == 0) {
            out << '\n';
        }
    }
}
