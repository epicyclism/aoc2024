#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <span>

#include "ctre_inc.h"
#include "timer.h"

int cnt;

struct Equation
{
    uint64_t result;
    std::vector<uint32_t> operands;
};

auto get_input()
{
	std::vector<Equation> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		rv.push_back({});
        bool r { true};
		for(auto v: ctre::search_all<"(\\d+)">(ln))
        {
            if(r)
            {
                rv.back().result = v.to_number<uint64_t>();
                r = false;
            }
            else
			    rv.back().operands.emplace_back(v.to_number<uint32_t>());                
        }
	}
	return rv;
}

// Recursive trinary search.
bool could_be_true2(const Equation& e, uint64_t result, int index)
{
    ++cnt;
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
    cnt = 0;
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
bool is_valid2(uint64_t result, std::span<const uint32_t> operands)
{
    auto l = operands.size();
    if (l == 1) {
        return result == operands[0];
    }
    uint32_t last = operands[l - 1];
    if (result % last == 0) {
        if (is_valid2(result / last, std::span<const uint32_t>(operands.begin(), operands.end() - 1))) {
            return true;
        }
    }
    auto sub = result - last;
    if (sub >= 0) {
        if (is_valid2(sub, std::span<const uint32_t>(operands.begin(), operands.end() - 1))) {
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
            if (is_valid2(sub / p, std::span<const uint32_t>(operands.begin(), operands.end() - 1))) {
                return true;
            }
        }
    }

    return false;
}

bool is_valid2a(uint64_t result, std::vector<uint32_t> const& operands, size_t l)
{
    ++cnt;
    if (l == 0) {
        return result == operands[0];
    }
    uint32_t last = operands[l];
    if (result % last == 0) {
        if (is_valid2a(result / last, operands, l - 1)) {
            return true;
        }
    }
    auto sub = result - last;
    if (sub >= 0) {
        if (is_valid2a(sub, operands, l - 1)) {
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
            if (is_valid2a(sub / p, operands, l - 1)) {
                return true;
            }
        }
    }

    return false;
}

uint64_t part2_rust(const std::vector<Equation>& equations)
{
    timer t("part2_rust");
    //aoc::timer timer;

    // Not too familiar with ranges/views, so used a boring old loop.
    uint64_t total{};
    for (const auto& [result, operands] : equations)
    {
        if (is_valid2(result, std::span<const uint32_t>{operands})) total += result;
    }
    return total;
}

uint64_t part2_b(const std::vector<Equation>& equations)
{
    timer t("part2_a");
    cnt = 0;
    //aoc::timer timer;

    // Not too familiar with ranges/views, so used a boring old loop.
    uint64_t total{};
    for (auto& e : equations)
    {
        if (is_valid2a(e.result, e.operands, e.operands.size() - 1)) total += e.result;
    }
    return total;
}

int main()
{
	auto in = get_input();
	std::cout << "pt2 = " << part2_cpp(in) << "(" << cnt << ")\n";
	std::cout << "pt2 = " << part2_rust(in) << "\n";
	std::cout << "pt2 = " << part2_b(in) << "(" << cnt << ")\n";
}
