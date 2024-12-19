#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "trie.h"
#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	trie_t<int> tt;
	std::vector<std::string> designs;
	int v{0};
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(tt.empty())
		{
			for (auto m : ctre::search_all<"([a-z]+)">(ln))
				tt.insert(m.to_view(), v++);
		}
		else
		if(!ln.empty())
			designs.emplace_back(ln);
	}

	return std::make_pair(tt, designs);
}

bool can_make(auto const& tt, std::string_view design, int& cnt)
{
	++cnt;
	if(design.empty())
		return true;
	if(cnt > 50)
		return false;
	auto dpth { tt.find_depth(design)};
	if(dpth)
	{
		std::string_view dd{design.data(), dpth};
		while(!dd.empty())
		{
			if(tt.find(dd))
			{
				std::string_view ddd{design};
				ddd.remove_prefix(dd.size());
				if(can_make(tt, ddd, cnt))
					return true;
			}
			dd.remove_suffix(1);
		}	
	}
	return false;
}

int64_t pt1(auto const& tt, auto const& designs)
{
	timer t("pt1");
	return std::count_if(designs.begin(), designs.end(), [&](auto& design)
	{
		int c{0};
		auto cm { can_make(tt, design, c)};
		return cm;
	});
}

int64_t how_many(auto const& tt, std::string_view design, auto& cache)
{
	if(design.empty())
		return 1;
	if(auto ht = cache.find(design); ht)
		return ht.value();
	int64_t sm{0};
	auto dpth { tt.find_depth(design)};
	if(dpth)
	{
		std::string_view dd{design.data(), dpth};
		while(!dd.empty())
		{
			if(tt.find(dd))
			{
				std::string_view ddd{design};
				ddd.remove_prefix(dd.size());
				auto cnt = how_many(tt, ddd, cache);
				cache.insert(ddd, cnt);
				sm += cnt;
			}
			dd.remove_suffix(1);
		}	
	}
	return sm;
}

int64_t pt2(auto const& tt, auto const& designs)
{
	timer t("pt2");
	trie_t<int64_t> cache;
	return std::reduce(designs.begin(), designs.end(), 0LL,[&](int64_t sm, auto& design)
	{
		int c{0};
		auto cm { can_make(tt, design, c)};
		if(cm)
			sm += how_many(tt, design, cache);
		return sm;
	});
}

int main()
{
	auto [dict, designs] = get_input();
	auto p1{ pt1(dict, designs) };
	std::cout << "pt1 = " << p1 << "\n";
	auto p2{ pt2(dict, designs) };
	std::cout << "pt2 = " << p2 << "\n";
}
