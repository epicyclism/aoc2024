cmake_minimum_required (VERSION 3.21)

add_executable(aoc7 aoc7.cpp)
target_include_directories(aoc7 PUBLIC ../common)
target_link_libraries(aoc7 PRIVATE ctre::ctre)

add_executable(aoc7r aoc7r.cpp)
target_include_directories(aoc7r PUBLIC ../common)
target_link_libraries(aoc7r PRIVATE ctre::ctre)

add_executable(aoc7rd aoc7rd.cpp)
target_include_directories(aoc7rd PUBLIC ../common)
target_link_libraries(aoc7rd PRIVATE ctre::ctre)
