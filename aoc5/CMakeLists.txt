cmake_minimum_required (VERSION 3.21)

add_executable(aoc5 aoc5.cpp)
target_include_directories(aoc5 PUBLIC ../common)
target_link_libraries(aoc5 PRIVATE ctre::ctre)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
add_executable(aoc5_brute aoc5_brute.cpp)
target_include_directories(aoc5_brute PUBLIC ../common)
target_link_libraries(aoc5_brute PRIVATE ctre::ctre)
add_executable(aoc5_par aoc5_par.cpp)
target_include_directories(aoc5_par PUBLIC ../common)
target_link_libraries(aoc5_par PRIVATE ctre::ctre)
endif()