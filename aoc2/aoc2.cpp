#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

auto get_input()
{
	std::vector<std::vector<int>> rv;

	std::string ln;
	while(std::getline(std::cin, ln))
	{
		rv.push_back({});
		for (auto m : ctre::search_all<"(\\d+)">(ln))
			rv.back().emplace_back(m.to_number<int>());
	}
	return rv;
}

int pt1(auto const& in)
{
	int cnt{0};
	std::vector<int> wsp;
	for(auto& v : in)
	{
		wsp.resize(v.size());
		std::adjacent_difference(v.begin(), v.end(), wsp.begin());
		auto[mn, mx] = std::minmax_element(wsp.begin() + 1, wsp.end());
		cnt += (*mn > 0 && *mx < 4 ) || (*mn > -4 && *mx < 0);
	}

	return cnt;
}

auto pt12(auto const& in)
{
	int cnt1{0};
	int cnt2{0};
	std::vector<int> wsp;
	std::vector<int> wsp2;
	for(auto& v : in)
	{
		wsp.resize(v.size());
		std::adjacent_difference(v.begin(), v.end(), wsp.begin());
		auto[mn, mx] = std::minmax_element(wsp.begin() + 1, wsp.end());
		if((*mn > 0 && *mx < 4 ) || (*mn > -4 && *mx < 0))
			++cnt1;
		else
			for(int n { 0}; n < v.size(); ++n)
			{
				wsp2 = v;
				wsp2.erase(wsp2.begin() + n);
				std::adjacent_difference(wsp2.begin(), wsp2.end(), wsp.begin());
				auto[mn, mx] = std::minmax_element(wsp.begin() + 1, wsp.end() - 1);
				cnt2 += (*mn > 0 && *mx < 4 ) || (*mn > -4 && *mx < 0);
			}
	}

	return std::make_pair(cnt1, cnt1 + cnt2);
}

int main()
{
	auto in { get_input()};
	auto[p1, p2] = pt12(in);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
