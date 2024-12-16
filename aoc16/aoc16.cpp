#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <bitset>

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

inline int cost_swivel(int df, int dt)
{
	if (df == dt)
		return 1;
	return std::abs(dt - df) == 2 ? 1000000 : 1001;
}

auto dijkstra(auto const& g, vertex_id_t from, vertex_id_t to)
{
	struct pq_t
	{
		vertex_id_t v_;
		int         dir_;
	};
	std::vector<int> distance(g.size(), -1);
	auto pq_t_cmp = [&](auto& l, auto& r) { return distance[l.v_] < distance[r.v_]; };
	std::priority_queue<pq_t, std::vector<pq_t>, decltype(pq_t_cmp)> q(pq_t_cmp);
	q.push({ from, 2 });
	distance[from] = 0;
	while (!q.empty())
	{
		auto p = q.top(); q.pop();
		int d{ 0 };
		for (auto e : g[p.v_])
		{
			if (valid_vertex_id(e) && (distance[e] == -1 || distance[e] > distance[p.v_] + cost_swivel(d, p.dir_)))
			{
				distance[e] = distance[p.v_] + cost_swivel(d, p.dir_);
				q.push({ e, d });
			}
			++d;
		}
	}

	return distance[to];
}

int64_t pt1(auto const& in, size_t str, size_t S, size_t E)
{
	timer t("pt1");
	grid_4 g(in, str, [](auto f, auto t) {return t != '#'; });

	return dijkstra(g, S, E);
}

auto bfs_x(auto const& g, vertex_id_t from, vertex_id_t to, size_t tgt)
{
	struct q_t
	{
		vertex_id_t v_;
		int         dir_;
		size_t		dist_;
		std::bitset<20000> visited_;
	};
	std::bitset<20000> all;
	std::vector<size_t> distances(g.size(), tgt);
	std::queue<q_t> q;
	q_t qt{ from, 2, 0};
	qt.visited_.set(from);
	distances[from] = 0;
	q.push(qt);
	while (!q.empty())
	{
		auto p = q.front(); q.pop();
		if (p.v_ == to && p.dist_ == tgt)
		{
			all |= p.visited_;
		}
		int d{ 0 };
		for (auto e : g[p.v_])
		{
			if (valid_vertex_id(e) && !p.visited_.test(e))
			{
				auto ed{ p.dist_ + cost_swivel(d, p.dir_) };
				if (ed <= distances[e] + 1000)
				{
					q_t qt{ e, d, ed, p.visited_ };
					qt.visited_.set(e);
					q.push(qt);
					distances[e] = ed;
				}
			}
			++d;
		}
	}

	return all.count();
}

int64_t pt2(auto const& in, size_t str, size_t S, size_t E, size_t tgt)
{
	timer t("pt2");
	grid_4 g(in, str, [](auto f, auto t) {return t != '#'; });

	return bfs_x(g, S, E, tgt);
}

int main()
{
	auto [v, s, S, E] = get_input();
	auto p1{ pt1(v, s, S, E) };
	std::cout << "pt1 = " << p1 << "\n";
	auto p2{ pt2(v, s, S, E, p1) };
	std::cout << "pt2 = " << p2 << "\n";
}