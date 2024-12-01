# aoc2024
Advent Of Code 2024

aoc_test contains some old solutions from previous years and isn't pertinent.

Each new day of 2024 will get its own directory. Builds should be good on MacOS, Windows and Linux.

For reference,

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/home/paul/vcpkg/scripts/buildsystems/vcpkg.cmake

(vcpkg path according to situation and whether anything like Boost is actually needed...)
