#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

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

auto pt12(auto const& tt, auto const& designs)
{
	timer t("pt12");
	trie_t<int64_t> cache;
	return std::reduce(designs.begin(), designs.end(), std::make_pair(0LL, 0LL),[&](auto sm, auto& design)
	{	
		auto t = how_many(tt, design, cache);
		sm.first += t != 0;
		sm.second += t;
		return sm;
	});
}

int main()
{
	auto [dict, designs] = get_input();

	auto[p1, p2] = pt12(dict, designs);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
