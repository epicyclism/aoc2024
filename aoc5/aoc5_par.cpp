#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <execution>

#include "ctre_inc.h"
#include "linear_set.h"
#include "timer.h"

auto get_input()
{
	std::vector<linear_set<unsigned char>> rules(100);
	std::vector<std::vector<unsigned char>> lists;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			break;
		if(auto[m, a, b] = ctre::match<"(\\d+)\\|(\\d+)">(ln); m)
		{
			rules[b.to_number<int>()].emplace_back( a.to_number<unsigned char>());
		}
	}
	while(std::getline(std::cin, ln))
	{
		lists.push_back({});
		for (auto m : ctre::search_all<"(\\d+)">(ln))
			lists.back().emplace_back(m.to_number<unsigned char>());
	}

	return std::make_pair(rules, lists);
}

bool ordered (auto const& rules, auto const& l)
{
	for(int n{0}; n < l.size(); ++n)
	{
		for(int m{n + 1}; m < l.size(); ++m)
		{
			if(l[n] > 99)
				std::cout << l[n] << " oops\n";
			if(rules[l[n]].contains(l[m]))
				return false;
		}
	}
	return true;
}

std::pair<int, int> pt12(auto const& rules, auto& lists)
{
	timer t("pt12");
	int rv1{0};
	int rv2{0};

	typename std::remove_reference<decltype(lists)>::type lists2;
	for (auto& l : lists)
	{
		if (ordered(rules, l))
			rv1 += l[l.size() / 2];
		else
			lists2.emplace_back(l);
	}
	rv2 = std::transform_reduce(std::execution::par, lists2.begin(), lists2.end(), 0, std::plus<>(),
		[&](auto& l)
		{
			std::sort(l.begin(), l.end(), [&](auto le, auto re)
				{
					return !rules[re].contains(le);
				});
			return l[l.size() / 2];
		});
	return std::make_pair(rv1, rv2);
}

int main()
{
	auto [rules, lists] = get_input();
	auto[p1, p2] = pt12(rules, lists);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
