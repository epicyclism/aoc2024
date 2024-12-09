#include <cstdint>
#include <vector>
#include <span>
#include <iostream>

#include "timer.h"

// I don't usually do this.
using namespace std;


struct Equation
{
    uint64_t result;
    vector<uint32_t> operands;
};


// Recursive trinary search.
bool could_be_true2(const Equation& e, uint64_t result, int index)
{
    // Condition reduces part2 execution time.
    if (result > e.result) return false;

    if (index == e.operands.size())
        return e.result == result;

    // Return early rather than evaluate further. Cheaper operator first.
    if (could_be_true2(e, result + e.operands[index], index + 1)) return true;
    if (could_be_true2(e, result * e.operands[index], index + 1)) return true;

    // Concatentation operator.
    auto temp = e.operands[index];
    while (temp > 0)
    {
        result *= 10;
        temp /= 10;
    }
    return could_be_true2(e, result + e.operands[index], index + 1);
}


template <typename T>
auto part2_cpp(T& input)
{
    timer t("part2_cpp");

    //aoc::timer timer;

    uint64_t total{};
    for (const auto& e : input)
    {
        if (could_be_true2(e, e.operands[0], 1)) total += e.result;
    }

    return total;
}


// This Rust badly translated into C++ below
// pub fn sum_valid_equations_with_concat(equations: &[Equation]) -> i64 {
//     fn is_valid(result: i64, operands: &[i64]) -> bool {
//         let l = operands.len();
//         if l == 1 {
//             return result == operands[0];
//         }
//         let last = operands[l - 1];
//         if result % last == 0 {
//             if is_valid(result / last, &operands[0..l - 1]) {
//                 return true;
//             }
//         }
//         let sub = result - last;
//         if sub >= 0 {
//         if is_valid(sub, &operands[0..l - 1]) {
//             return true;
//         }

//         let p = 10i64.pow(nb_of_digits(last));
//         if sub % p == 0 {
//             if is_valid(sub / p, &operands[0..l - 1]) {
//                 return true;
//             }
//         }
//     }
//     false
//     }
//     equations
//         .iter()
//         .filter_map(|eq| {
//             if is_valid(eq.result, &eq.operands) {
//                 Some(eq.result)
//             } else {
//                 None
//             }
//         })
//         .sum()
// }


// This a local recursive method in Rust.
// Used span to simulate the way slices were used. Bad idea?
bool is_valid2(uint64_t result, span<const uint32_t> operands)
{
    auto l = operands.size();
    if (l == 1) {
        return result == operands[0];
    }
    uint32_t last = operands[l - 1];
    if (result % last == 0) {
        if (is_valid2(result / last, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
            return true;
        }
    }
    auto sub = result - last;
    if (sub >= 0) {
        if (is_valid2(sub, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
            return true;
        }

        auto p = 10;
        // Inlined the Rust code for this bit
        uint32_t temp = last;
        while (temp >= 10) {
            p *= 10;
            temp /= 10;
        }
        if (sub % p == 0) {
            if (is_valid2(sub / p, span<const uint32_t>(operands.begin(), operands.end() - 1))) {
                return true;
            }
        }
    }

    return false;
}


uint64_t part2_rust(const vector<Equation>& equations)
{
    timer t("part2_rust");
    //aoc::timer timer;

    // Not too familiar with ranges/views, so used a boring old loop.
    uint64_t total{};
    for (const auto& [result, operands] : equations)
    {
        if (is_valid2(result, span<const uint32_t>{operands})) total += result;
    }
    return total;
}


void run(const char* filename)
{
    // Read from a file with lines in the form <result>: <operand> [<operand> ...]. 
    // Number of operands range from 1 to a dozen or so.
    // Example: "12347329: 2155 21 545 3 9 1 3 82 2"
    // Determine whether result is achievable by an expression involving all operands (in order). 
    // Evaluate left to right. Ignore precedence. See https://adventofcode.com/2024/day/7 for operators.

    // Example input from AoC problem. Expecte result: 11387.
    // Real input 100s of entries and some result values are uint64_t.
    vector<Equation> equations =
    {
        { 190, { 10, 19 } },
        { 3267, { 81, 40, 27 } },
        { 83, {17, 5 } },
        { 156, {15, 6 } },
        { 7290, {6, 8, 6, 15 } },
        { 161011, {16, 10, 13 } },
        { 192, {17, 8, 14 } },
        { 21037, {9, 7, 18, 13 } },
        { 292, {11, 6, 16, 20 } },
    };

    auto p2c = part2_cpp(equations);
    cout << "Part 2 C++:  " << p2c << '\n';

    auto p2r = part2_rust(equations);
    cout << "Part 2 Rust: " << p2r << '\n';
}


int main(int argc, char** argv)
{
    run("");
}