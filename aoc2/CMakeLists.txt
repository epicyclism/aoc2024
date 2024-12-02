cmake_minimum_required (VERSION 3.21)

add_executable(aoc2 aoc2.cpp)
target_include_directories(aoc2 PUBLIC ../common)
target_link_libraries(aoc2 PRIVATE ctre::ctre)

add_executable(aoc2bb aoc2bb.cpp)
target_include_directories(aoc2bb PUBLIC ../common)
target_link_libraries(aoc2bb PRIVATE ctre::ctre)