#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int64_t> vl;
	std::vector<int64_t> vr;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, a, b] = ctre::match<"(\\d+)\\s+(\\d+)">(ln); m)
		{
			vl.emplace_back(a.to_number<int64_t>());
			vr.emplace_back(b.to_number<int64_t>());
		}
	}
	std::ranges::sort(vl);
	std::ranges::sort(vr);

	return std::make_pair(vl, vr);
}

int64_t pt1(auto const& l, auto const& r)
{
	timer t("p1");
	return std::transform_reduce(l.begin(), l.end(), r.begin(), 0LL, std::plus<>(), [](auto l, auto r)
		{
			return std::abs(l - r);
		});
}

int64_t pt2(auto const& vl, auto const& vr)
{
	timer t("p2");
	int64_t v { 0};
	for(auto l: vl)
	{
		v += l * std::count(vr.begin(), vr.end(), l);
	}
	return v;
}

int main()
{
	auto [l, r] = get_input();
	auto p1 = pt1(l, r);
	auto p2 = pt2(l, r);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
