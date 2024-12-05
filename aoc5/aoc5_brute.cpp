#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <execution>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::map<int, std::set<int>> rules;
	std::vector<std::vector<int>> lists;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			break;
		if(auto[m, a, b] = ctre::match<"(\\d+)\\|(\\d+)">(ln); m)
		{
			rules[b.to_number<int>()].emplace( a.to_number<int>());
		}
	}
	while(std::getline(std::cin, ln))
	{
		lists.push_back({});
		for (auto m : ctre::search_all<"(\\d+)">(ln))
			lists.back().emplace_back(m.to_number<int>());
	}

	return std::make_pair(rules, lists);
}

bool ordered (auto const& rules, auto const& l)
{
	for(int n{0}; n < l.size(); ++n)
	{
		for(int m{n + 1}; m < l.size(); ++m)
		{
			if(rules.contains(l[n]))
			{
				if(rules.at(l[n]).contains(l[m]))
					return false;
			}
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
				std::sort(l.begin(), l.end());
				while (std::next_permutation(l.begin(), l.end()))
				{
					if (ordered(rules, l))
						return l[l.size() / 2];
				}
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
