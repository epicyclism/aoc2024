#include <iostream>
#include <vector>
#include <string>
#include <array>

#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<char> v;
	std::string ln;
	size_t str{0};
	size_t S{0};
	size_t E{0};
	size_t P{0};
	while(std::getline(std::cin, ln))
	{
		if(str == 0)
			str = ln.size();
		if(auto p = ln.find('S'); p != std::string::npos)
			S = P + p;
		if(auto p = ln.find('E'); p != std::string::npos)
			E = P + p;
		P += ln.size();
		v.insert(v.end(), ln.begin(), ln.end());
	}

	return std::make_tuple(v, str, S, E);
}


int64_t pt1(auto const& in, size_t str, size_t S, size_t E)
{
	timer t("pt1");
	grid gd(in, str, [](auto f, auto t){ return t != '#';});
	auto r = bfs<decltype(gd), true>(gd, S);

	auto base_distance{r.first[E]};
	int cnt{0};
	auto p = E;
	while( p != S)
	{
		auto cheat_start { r.first[p]};
		for(auto e : gd.two_step(p))
		{
			if(valid_vertex_id(r.first[e]))
			{
				auto saved = cheat_start - r.first[e] - 2;
				if(saved > 99)
					++cnt;
			}
		}
		p = r.second[p];
	}
	return cnt;
}

int64_t pt2(auto const& in, size_t str, size_t S, size_t E)
{
	timer t("pt2");
	grid gd(in, str, [](auto f, auto t){ return t != '#';});
	auto r = bfs<decltype(gd), true>(gd, S);

	auto base_distance{r.first[E]};
	int cnt{0};
	std::array<int, 50> rs;
	rs.fill(0);
	auto p = E;
	while( p != S)
	{
		auto cheat_start { r.first[p]};
		for(auto [e, d] : gd.template n_step<20>(p))
		{
			if(valid_vertex_id(r.first[e]))
			{
				auto saved = cheat_start - r.first[e] - d;
				if(saved > 49)
				{
					++cnt;
					++rs[saved-50];
				}
			}
		}
		p = r.second[p];
	}
	for(int n{0}; n < 50; ++n)
		if(rs[n])
			std::cout << rs[n] << " - " << n + 50 << "\n";
	return cnt;
}

int main()
{
	auto [v, s, S, E] = get_input();

	auto p1{ pt1(v, s, S, E) };
	std::cout << "pt1 = " << p1 << "\n";
	auto p2{ pt2(v, s, S, E) };
	std::cout << "pt2 = " << p2 << "\n";
}

// p1 1269 too high
// p2 952515 too low