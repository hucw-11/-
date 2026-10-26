cmake_minimum_required(VERSION 3.16)
project(FifteenPuzzle LANGUAGES CXX)

add_executable(fifteen_puzzle main.cpp puzzle.cpp astar.cpp)
target_compile_features(fifteen_puzzle PRIVATE cxx_std_17)
set_target_properties(fifteen_puzzle PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC)
    target_compile_options(fifteen_puzzle PRIVATE /W4 /utf-8)
else()
    target_compile_options(fifteen_puzzle PRIVATE -Wall -Wextra -Wpedantic)
endif()
