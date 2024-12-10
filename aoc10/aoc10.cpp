#include <iostream>
#include <vector>
#include <string>
//#include <algorithm>
//#include <numeric>
#include <queue>

#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<char> r;
	std::string ln;
	size_t stride{ 0 };
	while (std::getline(std::cin, ln))
	{
		if(stride == 0) stride = ln.size();
		r.insert(r.end(), ln.begin(), ln.end());
	}
	return std::make_pair(stride, r);
}

int score_trailhead(std::size_t h, auto& g)
{
	int cnt{0};
	std::queue<vertex_id_t> q;
	std::vector<bool> visited(g.size());
    q.push(h);
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        for (auto e : g[u])
        {
			if(!visited[e])
			{
				visited[e] = true;
				if(g.value(e) == '9')
					++cnt;
				else
					q.push(e);
			}
        }
    }
	return cnt;
}

int rate_trailhead(std::size_t h, auto& g)
{
	int cnt{0};
	std::queue<vertex_id_t> q;
    q.push(h);
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        for (auto e : g[u])
        {
			if(g.value(e) == '9')
				++cnt;
			else
				q.push(e);
        }
    }
	return cnt;
}

int pt1(auto const& in)
{
	timer t{"pt1"};
	int r { 0 };
	grid_direct gd(in.second, in.first, [&](auto f, auto t){return in.second[t] == in.second[f] + 1;});
	for(std::size_t p { 0 }; p < in.second.size(); ++p)
		if(in.second[p] == '0')
			r += score_trailhead(p, gd);

	return r;
}

int64_t pt2(auto const& in)
{
	timer t("pt2");
	int r { 0 };
	grid_direct gd(in.second, in.first, [&](auto f, auto t){return in.second[t] == in.second[f] + 1;});
	for(std::size_t p { 0 }; p < in.second.size(); ++p)
		if(in.second[p] == '0')
			r += rate_trailhead(p, gd);

	return r;
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}