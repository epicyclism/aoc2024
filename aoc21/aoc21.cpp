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
	timer t("pt1");
	return std::transform_reduce(l.begin(), l.end(), r.begin(), 0LL, std::plus<>(), [](auto l, auto r)
		{
			return std::abs(l - r);
		});
}

int64_t pt2(auto const& vl, auto const& vr)
{
	timer t("pt2");
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
	std::cout << "pt1 = " << pt1(l, r) << "\n";
	std::cout << "pt2 = " << pt2(l, r) << "\n";
}
