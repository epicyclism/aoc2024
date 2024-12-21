#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>

#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::vector<char>> vv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		std::vector<char> v;
		for(auto c: ln)
		{
			if(c == 'A')
				v.push_back(0xa);
			else
				v.push_back(c - '0');
		}
		vv.emplace_back(v);
	}
	return vv;
}

char to_char_dir(char d)
{
	char c{'!'};
	switch(d)
	{
		case 0:
			c = '<';
			break;
		case 1:
			c = '^';
			break;
		case 2:
			c = '>';
			break;
		case 3:
			c = 'v';
			break;
		case 0xa:
			c = 'A';
			break;
		default:
			break;
	}
	return c;
}
void print(auto const& v)
{
	std::cout << v.size() << " : ";
	for(auto d: v)
	{
		std::cout << to_char_dir(d);
	}
	std::cout << "\n";
}

auto bfs(auto const& g, char id_from) 
{
	std::vector<std::pair<char, char>> 		arrived_from(g.size());
    std::vector<bool>       visited(g.size());
    std::queue<char> q;
    q.push(id_from);
	arrived_from[id_from] = {id_from, 0};
    visited[id_from] = true;
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		auto& eg{g[u]};
		char d{arrived_from[u].second};
		for(int n = 0; n < 4; ++n)
		{
        	if (valid_vertex_id(eg[d]) && !visited[eg[d]])
        	{
                visited[eg[d]] = true;
				arrived_from[eg[d]] = {u, d};
               q.push(eg[d]);
            }
			++d;
			d &= 3;
		}
    }
    return arrived_from;
}

inline int cost_swivel(int df, int dt)
{
	if (df == dt)
		return 1;
	if(df == 8)
		return 1;
	return std::abs(dt - df) == 2 ? 1000000 : 2;
}

auto dijkstra(auto const& g, char from)
{
	struct pq_t
	{
		char v_;
		char dir_;
	};
	std::vector<std::pair<char, char>> 		arrived_from(g.size());
	std::vector<int> distance(g.size(), -1);
	auto pq_t_cmp = [&](auto& l, auto& r) { return distance[l.v_] < distance[r.v_]; };
	std::priority_queue<pq_t, std::vector<pq_t>, decltype(pq_t_cmp)> q(pq_t_cmp);
	q.push({ from, 8 });
	distance[from] = 0;
	while (!q.empty())
	{
		auto p = q.top(); q.pop();
		char d{ 0 };
		for (auto e : g[p.v_])
		{
			if (valid_vertex_id(e) && (distance[e] == -1 || distance[e] > distance[p.v_] + cost_swivel(d, p.dir_)))
			{
				distance[e] = distance[p.v_] + cost_swivel(d, p.dir_);
				arrived_from[e] = {e, p.dir_};
				q.push({ e, d });
			}
			++d;
		}
	}

	return arrived_from;
}
std::vector<char> route(auto const& prev, char start, char end)
{
	std::vector<char> route;
	auto p{end};
	while(p!=start)
	{
		route.push_back(prev[p].second);
		p = prev[p].first;
	}
	std::reverse(route.begin(), route.end());
	return route;
}

// 789
// 456
// 123
//  0A
//
std::vector<std::array<char, 4>> keypad
{
	{-1, 2, 0xa, -1}, // 0
	{-1, 4, 2, 0}, // 1
	{1, 5, 3, 0}, // 2
	{2, 6, -1, 0xa}, // 3
	{-1, 7, 5, 1}, // 4
	{4, 8, 6, 2}, // 5
	{5, 9, -1, 3}, // 6
	{-1, -1, 8, 4}, // 7
	{7, -1, 9, 5}, // 8
	{8, -1, -1, 6}, // 9
	{0, 3, -1, -1}  // A
};

std::vector<char> command_keypad(auto const& s)
{
	std::vector<char> rv;
	char start {0xa};
	for(auto c: s)
	{
		auto r = dijkstra(keypad, start);
		auto rt{route(r, start, c)};
		rv.insert(rv.end(), rt.begin(), rt.end());
		rv.push_back(0xa);
		start = c;
	}
	return rv;
}

//  ^A    1 a
// <v>  0 3 2
//
std::vector<std::array<char, 4>> directionpad
{
	{-1, -1, 3, -1},  // 0
	{-1, -1, 0xa, 3}, // 1
	{3, 0xa, -1, -1}, // 2
	{0, 1, 2, -1}, // 3
	{-1, -1, -1, -1}, // 4
	{-1, -1, -1, -1}, // 5	
	{-1, -1, -1, -1}, // 6
	{-1, -1, -1, -1}, // 7
	{-1, -1, -1, -1}, // 8
	{-1, -1, -1, -1}, // 9
	{1, -1, -1, 2},   // a
};

std::vector<char> command_directionpad(auto const& s)
{
	std::vector<char> rv;
	char start {0xa};
	for(auto c: s)
	{
		auto r = dijkstra(directionpad, start);
		auto rt{route(r, start, c)};
		rv.insert(rv.end(), rt.begin(), rt.end());
		rv.push_back(0xa);
		start = c;
	}
	return rv;
}

void decode_directionpad(auto const& v)
{
	char now{0xa};
	for(auto c: v)
	{
		switch(c)
		{
			case 0:
			if(now == 3)
				now = 0;
			else
			if(now == 2)
				now = 3;
			else
			if(now == 0xa)
				now = 1;
			break;
			case 1:
			if(now == 3)
				now = 1;
			else
			if(now == 2)
				now = 0xa;
			break;
			case 2:
				if(now == 0)
					now = 3;
				else
				if(now == 3)
					now = 2;
				else if(now == 1)
					now = 0xa;
			break;
			case 3:
				if(now == 1)
					now = 3;
				else
				if(now == 0xa)
					now = 2;
			break;
			case 0xa:
				std::cout << to_char_dir(now);
			break;
			default:
			break;
		}
	}
	std::cout << "\n";
}

int64_t to_i(auto& v)
{
	int64_t t{0};
	for(auto c: v)
	{
		if(c>9)
			break;
		t *= 10;
		t += c;
	}
	return t;
}

int64_t pt1(auto const& vv)
{
	timer t("pt1");
	int64_t r{0};
	for(auto& v: vv)
	{
		auto v1{command_keypad(v)};
		print(v1);
		auto v2{command_directionpad(v1)};
		print(v2);
		decode_directionpad(v2);
		auto v3{command_directionpad(v2)};
		print(v3);
		decode_directionpad(v3);
//		auto v4{command_directionpad(v3)};
//		print(v4);
		std::cout << v3.size() << "\n";
		r += v3.size() * to_i(v);
	}
	return r;
}

int64_t pt2(auto const& vv)
{
	timer t("pt2");
	return 0;
}

int main()
{
	auto vv = get_input();

	auto p1{pt1(vv)};
	std::cout << "pt1 = " << p1 << "\n";

	auto p2{pt2(vv)};
	std::cout << "pt2 = " << p2 << "\n";
}

// <v<A>^>A<A>A<AA>vA^A<vAAA^>A
// <v<A>A<A>^>AvA<^A>vA^A<v<A>^>AvA^A<v<A>^>AAvA<A^>A<A>A<v<A>A^>AAA<A>vA^A
//
// v<<A>>^A<A>AvA<^AA>A<vAAA>^A
// <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A
