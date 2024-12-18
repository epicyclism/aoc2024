#include <iostream>
#include <vector>
#include <string>

#include "ctre_inc.h"
#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<int, int>> v;
	std::string ln;
	while(std::getline(std::cin, ln))
		if(auto[m, a, b] = ctre::match<"(\\d+),(\\d+)">(ln); m)
			v.emplace_back(a.to_number<int>(), b.to_number<int>());

	return v;
}

// count, exit
std::pair<int, int> params(auto const& v)
{
	for(auto p: v)
		if(p.first > 6 || p.second > 6)
			return {1024, 5040};
	return {12, 48};
}

int pt1(auto const& in)
{
	timer t("pt1");
	auto[cnt, exit] = params(in);
	std::vector<char> g(71 * 71, '.');
	auto p {in.begin()};
	for(int n { 0}; n < cnt; ++n)
	{
		g[(*p).first + (*p).second * 71] = '#';
		++p;
	}
	grid gd(g, 71, [](auto f, auto t){ return t != '#';});
	auto r = bfs(gd, 0);
	return r[exit];
}

// looking for the first byte that obstructs the path,
// on the basis it is likely to be near the end of the input
// add all the blocks then remove and test from the back
// (just for speed)
std::pair<int, int> pt2(auto const& in)
{
	timer t("pt2");
	auto[cnt, exit] = params(in);
	std::vector<char> g(71 * 71, '.');
	for(auto& p: in)
		g[p.first + p.second * 71] = '#';
	grid gd(g, 71, [](auto f, auto t){ return t != '#';});
	auto pi{in.rbegin()};
	while(pi != in.rend())
	{
		g[(*pi).first + (*pi).second * 71] = '.';
		auto r = bfs(gd, 0);
		if(r[exit] != -1)
			break;
		++pi;
	}
	return *pi;
}

int main()
{
	auto in = get_input();
	auto p1{pt1(in)};
	std::cout << "pt1 = " << p1 << "\n";
	auto p2{pt2(in)};
	std::cout << "pt2 = " << p2.first << "," << p2.second << "\n";
}
