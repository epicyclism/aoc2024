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
std::vector<std::pair<int, std::vector<char>>> kp_crib
{
	{
		{ 29, {0, 0xa, 1, 0xa, 1, 1, 2, 0xa, 3, 3, 3, 0xa}},
		{980, {1, 1, 1, 0xa, 0, 0xa, 3, 3, 3, 0xa, 2, 0xa}},
		{179, {1, 0, 0, 0xa, 1, 1, 0xa, 2, 2, 0xa, 3, 3, 3, 0xa}},
		{456, {1, 1, 0, 0, 0xa, 2, 0xa, 2, 0xa, 3, 3, 0xa}},
		{379, {1, 0xa, 0, 0, 1, 1, 0xa, 2, 2, 0xa, 3, 3, 3, 0xa}},
		{382, {1, 0xa, 0, 1, 1, 0xa, 3, 3, 0xa, 3, 2, 0xa}},
		{176, {1, 0, 0, 0xa, 1, 1, 0xa, 3, 2, 2, 0xa, 3, 3, 0xa}},
		{463, {1, 1, 0, 0, 0xa, 2, 2, 0xa, 3, 0xa, 3, 0xa}},
		{ 83, {0, 0xa, 1, 1, 1, 0xa, 3, 3, 2, 0xa, 3, 0xa}},
		{789, {1, 1, 1, 0, 0, 0xa, 2, 0xa, 2, 0xa, 3, 3, 3, 0xa}}
	}
};

std::vector<char> command_keypad(auto const& s, int k)
{
	std::vector<char> rv;
	for(auto& kpc: kp_crib)
	{
		if( kpc.first == k)
		{
			std::cout << "kpc hit " << k << "\n";
			return kpc.second;
		}
	}
	char start {0xa};
	for(auto c: s)
	{
		auto r = bfs(keypad, start);
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

std::vector<std::vector<std::vector<char>>> dp_data
{
	// from 0
	{{}, {2, 1}, {2, 2}, {2}, {2, 2, 1}},
	{{3, 0}, {}, {3, 2}, {3}, {2}},
	{{0, 0}, {1, 0}, {}, {0}, {1}},
	{{0}, {1}, {2}, {}, {2, 1}},
	{{3, 0, 0}, {0}, {3}, {3, 0}, {}},
};

std::vector<char> route_dp(char start, char end)
{
	return dp_data[start == 0xa ? 4 : start][end == 0xa ? 4: end];
}

std::vector<char> command_directionpad(auto const& s)
{
	std::vector<char> rv;
	char start {0xa};
	for(auto c: s)
	{
		auto rt{route_dp(start, c)};
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
		int id = to_i(v);
		auto v1{command_keypad(v, id)};
		auto v2{command_directionpad(v1)};
		auto v3{command_directionpad(v2)};
		r += v3.size() * id;
	}
	return r;
}

int64_t pt2(auto const& vv)
{
	timer t("pt2");
	int64_t r{0};
	for(auto& v: vv)
	{
		int id = to_i(v);
		auto v1{command_keypad(v, id)};
		auto v2 = v1;
		auto sz{v2.size()};
		for(int n = 0; n < 25; ++n)
		{
			v2 = command_directionpad(v2);
			std::cout << n << " " << v2.size() << " (" << v2.size () - sz <<"\n";
			sz = v2.size();
		}
		r += v2.size() * id;
	}
	return r;
}

std::vector<char> translate(auto const& a)
{
	std::vector<char> rv;
	for(auto c: a)
	{
		if( c == '<')
			rv.push_back(0);
		if( c == '^')
			rv.push_back(1);
		if( c == '>')
			rv.push_back(2);
		if(c == 'v')
			rv.push_back(3);
		if( c == 'A')
			rv.push_back(0xa);
	}
	return rv;
}
int main()
{
	auto vv = get_input();

	auto p1{pt1(vv)};
	std::cout << "pt1 = " << p1 << "\n";

	auto p2{pt2(vv)};
	std::cout << "pt2 = " << p2 << "\n";

	std::string s = "<v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A";
	auto v = translate(s);
	std::cout << s << "\n";
	decode_directionpad(v);
}

// <v<A>^>A<A>A<AA>vA^A<vAAA^>A
// <v<A>A<A>^>AvA<^A>vA^A<v<A>^>AvA^A<v<A>^>AAvA<A^>A<A>A<v<A>A^>AAA<A>vA^A
//
// v<<A>>^A<A>AvA<^AA>A<vAAA>^A
// <v<A>>^AvA^A<vA<   AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A >^AAAvA<^A>A
// v<<A>>^AvA^Av<<A>>^AAv<A<A>>^AAvAA<^A>Av<A>^AA<A>Av<A<A>>^AAAvA<^A>A
//
// 133054 too high
// 132350 too high
// 132018 too high
// 130490 ??
// 128962 ??