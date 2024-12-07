#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::vector<uint64_t>> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		std::vector<uint64_t> e;
		for(auto v: ctre::search_all<"(\\d+)">(ln))
			e.emplace_back(v.to_number<uint64_t>());
		rv.emplace_back(std::move(e));
	}
	return rv;
}

bool eval(std::vector<uint64_t> const& v, size_t n, uint64_t val)
{
    if(val > v[0])
        return false;
    if(n == v.size())
        return v[0] == val;
    auto c {v[n]};
    return eval(v, n + 1, val * c) || eval(v, n + 1, val + c);
}

inline uint64_t ashift(uint64_t a, uint64_t b)
{
	while(b)
	{
		a*=10;
		b/=10;
	}
	return a;
}

bool eval2(std::vector<uint64_t> const& v, size_t n, uint64_t val)
{
    if(val > v[0])
        return false;
    if(n == v.size())
        return v[0] == val;
    auto c {v[n]};
    return eval2(v, n + 1, val * c) || eval2(v, n + 1, ashift(val, c) + c) || eval2(v, n + 1, val + c);
}

uint64_t pt1(auto const& in)
{
	timer t ("p1");
    return std::reduce(in.begin(), in.end(), 0ULL, [](auto a, auto& v)
    {
        return a + (eval(v, 2, v[1]) ? v[0] : 0);
    });
}

int64_t pt2(auto const& in)
{
	timer t ("p2");
    return std::reduce(in.begin(), in.end(), 0ULL, [](auto a, auto& v)
    {
        return a + (eval2(v, 2, v[1]) ? v[0] : 0);
    });
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
