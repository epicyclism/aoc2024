#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "ctre_inc.h"

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

#if 0
//#if defined(__cpp_lib_ranges_fold)
int64_t pt1(auto const& l, auto const& r)
{
	auto difference = [](auto l, auto r)
	{
		return std::abs(l - r);
	};
	return std::ranges::fold_left_first(std::views::zip_transform(difference, l, r), std::plus<>()).value().value();
}
#else
int64_t pt1(auto const& l, auto const& r)
{
	return std::transform_reduce(l.begin(), l.end(), r.begin(), 0LL, std::plus<>(), [](auto l, auto r)
	{
		return std::abs(l - r);
	});
}
#endif

int64_t pt2(auto const& vl, auto const& vr)
{
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
