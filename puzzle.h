#ifndef PUZZLE_H
#define PUZZLE_H

#include <array>
#include <iosfwd>
#include <vector>

constexpr int BOARD_WIDTH = 4;
constexpr int BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;
using Board = std::array<int, BOARD_SIZE>;

// 方向始终表示空格的移动方向。
enum class Move { Up, Down, Left, Right };

struct Neighbor {
    Board board;
    Move move;
};

bool isValidBoard(const Board& board);
bool isSolvable(const Board& start, const Board& goal);
int manhattanDistance(const Board& board, const Board& goal);
std::vector<Neighbor> getNeighbors(const Board& board);
const char* moveName(Move move);
void printBoard(const Board& board, std::ostream& out);

#endif
