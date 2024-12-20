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

struct p
{
	int d_;
	int x_;
	int y_;
};

inline int manhattan( p& f, p& t)
{
	return std::abs(f.x_ - t.x_) + std::abs(f.y_ - t.y_);
}

std::pair<int, int> pt12(auto const& in, size_t str, size_t S, size_t E)
{
	timer t("pt12");
	grid gd(in, str, [](auto f, auto t){ return t != '#';});
	auto r = bfs<decltype(gd), true>(gd, S);
	std::vector<p> vp;
	auto pt{E};
	while(1)
	{
		auto[x, y] = gd.to_xy(pt);
		vp.emplace_back(r.first[pt], x, y);
		if(pt == S)
			break;
		pt = r.second[pt];
	}
	int p1{0};
	int p2{0};
	for(auto is{vp.rbegin()}; is != vp.rend(); ++is)
		for(auto it{is + 1}; it != vp.rend(); ++it)
		{
			auto d { manhattan(*is, *it)};
			if(d == 2 && (*it).d_ - (*is).d_ - 2 > 99)
				++p1;
			if(d < 21 && (*it).d_ - (*is).d_ - d > 99)
				++p2;
		}
	return {p1, p2};
}

int main()
{
	auto [v, s, S, E] = get_input();

	auto[p1, p2] = pt12(v, s, S, E);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n"; 
}