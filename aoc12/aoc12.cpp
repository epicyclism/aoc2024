#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<char> r;
	std::string ln;
	size_t stride{ 0 };
	while (std::getline(std::cin, ln))
	{
		if(stride == 0)
		{
			 stride = ln.size() + 2;
			 r.insert(r.end(), stride, '_');
		}
		r.push_back('_');
		r.insert(r.end(), ln.begin(), ln.end());
		r.push_back('_');
	}
	 r.insert(r.end(), stride, '_');
	return std::make_pair(stride, r);
}

std::pair<int, int> trace_region(auto& gd, vertex_id_t s, std::vector<bool>& visited)
{
	int a{1};
	int p{0};
    std::queue<vertex_id_t> q;
    q.push(s);
	visited[s] = true;
	auto us{gd.value(s)};
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		int pp = 4;
        for (auto e : gd[u])
        {
			if (!visited[e])
			{
				++a;
				visited[e] = true;
					q.push(e);
			}
			--pp;
        }
		p += pp;
    }
	return {a, p};
}

std::pair<int, int> trace_region2(auto& gd, vertex_id_t s, std::vector<bool>& visited)
{
	int a{1};
	std::vector<std::pair<int, int>> sides[4];
    std::queue<vertex_id_t> q;
    q.push(s);
	visited[s] = true;
	auto us{gd.value(s)};
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		int ed{0};
        for (auto e : gd[u])
        {
			if(gd.value(e) == us)
			{
				if (!visited[e])
				{
					++a;
					visited[e] = true;
					q.push(e);
				}
			}
			else
			{
				if(ed < 2)
					sides[ed].emplace_back(int(e % gd.stride()), int(e / gd.stride()));
				else
					sides[ed].emplace_back(int(e / gd.stride()), int(e % gd.stride()));
			}
			++ed;
        }
    }
	int c{0};
	int dir{0};
	for(auto& sd : sides)
	{
		std::sort(sd.begin(), sd.end());
		std::pair<int, int> prev { -1, -1};
		for(auto& p : sd)
		{
			if( p.first != prev.first)
				++c;
			else
			if( p.second != prev.second + 1)
				++c;
			prev = p;
		}
		++dir;
	}
	return {a, c};
}

int pt1(auto const& in)
{
	timer t("pt1");
	std::vector<std::pair<int, int>> regions;
	grid_direct gd(in.second, in.first, [&](auto f, auto t){return in.second[f] == in.second[t];});
	std::vector<bool> visited(gd.size());
	for(vertex_id_t v{0}; v < gd.size(); ++v)
	{
		if(!visited[v] && gd.value(v) != '_')
			regions.push_back(trace_region(gd, v, visited));
	}
	return std::reduce(regions.begin(), regions.end(), 0, [](auto s, auto& r){
		return s + r.first * r.second;
	});
}

int pt2(auto const& in)
{
	timer t("pt2");
	std::vector<std::pair<int, int>> regions;
	grid_direct gd(in.second, in.first, [&](auto f, auto t){return true;});
	std::vector<bool> visited(gd.size());
	for(vertex_id_t v{0}; v < gd.size(); ++v)
	{
		if(!visited[v] && gd.value(v) != '_')
			regions.push_back(trace_region2(gd, v, visited));
	}
	return std::reduce(regions.begin(), regions.end(), 0, [](auto s, auto& r){
		return s + r.first * r.second;
	});
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
