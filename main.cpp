#include "astar.h"
#include "puzzle.h"

#include <exception>
#include <iostream>

int main() {
    // 0 表示空格。修改此处可以更换初始局面。
    const Board start = {
         1,  2,  3,  4,
         5,  0,  6,  8,
         9, 10,  7, 11,
        13, 14, 15, 12
    };
    const Board goal = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15,  0
    };

    try {
        std::cout << "15-puzzle: A* with Manhattan distance\n"
                  << "A dot (.) represents the blank. Moves refer to the blank.\n\n"
                  << "Initial board:\n";
        printBoard(start, std::cout);
        std::cout << "\nGoal board:\n";
        printBoard(goal, std::cout);

        const SearchResult result = solveAStar(start, goal);
        if (!result.solved) {
            std::cout << "\nNo solution exists for these boards.\n";
            return 1;
        }

        std::cout << "\nSolved! Minimum moves: " << result.moves.size() << '\n';
        std::cout << "Moves:";
        for (Move move : result.moves) {
            std::cout << ' ' << moveName(move);
        }
        std::cout << "\n\nSolution path:\n";
        for (std::size_t i = 0; i < result.path.size(); ++i) {
            std::cout << "Step " << i;
            if (i != 0) {
                std::cout << " (blank " << moveName(result.moves[i - 1]) << ')';
            }
            std::cout << ":\n";
            printBoard(result.path[i], std::cout);
            std::cout << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
