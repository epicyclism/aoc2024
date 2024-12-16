#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <numeric>

#include "graph.h"

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

struct vertex_t
{
	vertex_id_t to_;
	int wt_;
	vertex_t(vertex_id_t to, int w) : to_{to}, wt_{w}
	{}
};

using adjacency_list_t = std::vector<std::vector<vertex_t>>;

int cost_swivel(int df, int dt)
{
	return std::abs(dt - df) * 1000 + 1;
}

int count_out(auto const& a)
{
	return std::count_if(a.begin(), a.end(), [](auto v){ return v != -1;});
}

std::pair<vertex_id_t, int> first(auto const& a)
{
	for(int d{0}; d < a.size(); ++d)
		if(a[d] != -1)
			return {a[d], d};
	return {-1, 0};
}

// W, N, E, S : 0, 1, 2, 3
void follow(auto& g, auto const& gd, vertex_id_t f, int d, int wt)
{
	auto t{f};
	auto a { gd[t]};
	while(count_out(a) == 1)
	{
		auto[tt, dt] = first(a);
		t = tt;
		wt += cost_swivel(d, dt);
		d = dt;
		a = gd[t];
	}
//	std::cout << f << " " << t << " " << wt << "\n";
	add_edge_undirected(g, f, t, wt);
#if 1
	for(int nd{0}; nd < a.size(); ++nd)
	{
		if(a[nd] != -1)
			follow(g, gd, a[nd], nd, cost_swivel(d, nd));
	}
#endif
}

void print_graph(adjacency_list_t const& al)
{
	for(size_t v{ 0}; v < al.size(); ++v)
	{
		if(!al[v].empty())
		{
			std::cout << v << " -> " ;
			for(auto vv : al[v])
			{
				std::cout << "(" << vv.to_ << ", " << vv.wt_ << "), ";
			}
			std::cout << "\n";
		}
	}
}

adjacency_list_t make_graph( auto const& in, size_t str, size_t S)
{
	adjacency_list_t g;
	grid_4 gd(in, str, [](auto f, auto t){return t != '#';});
	follow(g, gd, S, 0, 0);
	print_graph(g);
	return g;
}

int64_t pt1(auto const& in, size_t str, size_t S, size_t E)
{
	std::cout << in.size() << " " << str << " " << S << " " << E << "\n";
	auto g{make_graph(in, str, S)};
	return 0;
}

int64_t pt2(auto const& in, size_t str)
{
	return 0;
}

int main()
{
	auto [v, s, S, E] = get_input();
	std::cout << "pt1 = " << pt1(v, s, S, E) << "\n";
	std::cout << "pt2 = " << pt2(v, s) << "\n";
}
