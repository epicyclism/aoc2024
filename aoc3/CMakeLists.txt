cmake_minimum_required (VERSION 3.21)

add_executable(aoc3 aoc3.cpp)
target_include_directories(aoc3 PUBLIC ../common)
target_link_libraries(aoc3 PRIVATE ctre::ctre)

add_executable(aoc3mm aoc3mm.cpp)
target_include_directories(aoc3mm PUBLIC ../common)
target_link_libraries(aoc3mm PRIVATE ctre::ctre)