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
	for(auto d: v)
	{
		std::cout << to_char_dir(d);
	}
	std::cout << "\n";
}

auto bfs(auto const& g, char id_from) 
{
    // abstract the differences in return data required
    using recorder_t = result_recorder_distance_previous<char, char>;

//    recorder_t              recorder(g.size());
	std::vector<std::pair<char, char>> 		arrived_from(g.size());
    std::vector<bool>       visited(g.size());
    std::queue<char> q;
    q.push(id_from);
//    recorder.set_distance(id_from, id_from);
    visited[id_from] = true;
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		char d{0};
        for (auto e : g[u])
        {

            if (valid_vertex_id(e) && !visited[e])
            {
                visited[e] = true;
				arrived_from[e] = {u, d};
//                recorder.set_distance(e, u);
//                recorder.set_previous(e, u);
                q.push(e);
            }
			++d;
        }
    }
    return arrived_from;
}

std::vector<char> route(auto const& prev, char start, char end)
{
#if 0
	std::cout << "route - " << +start << " " << +end << "\n";
	int n{0};
	for(auto& e: prev)
	{
		std::cout << n << " " << +e.first << " " << to_char_dir(e.second) << "\n";
		++n;
	}
	std::cout << "\n";
#endif
	std::vector<char> route;
	auto p{end};
	while(p!=start)
	{
		route.push_back(prev[p].second);
		p = prev[p].first;
	}
	std::reverse(route.begin(), route.end());
//	print(route);
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
		auto r = bfs(keypad, start);
		auto rt{route(r, start, c)};
		rv.insert(rv.end(), rt.begin(), rt.end());
		rv.push_back(0xa);
		start = c;
	}
	return rv;
}

//  ^A    3 a
// <v>  0 1 2
//
std::vector<std::array<char, 4>> directionpad
{
	{-1, -1, 1, -1},  // 0
	{0, 3, 2, -1},    // 1
	{1, 0xa, -1, -1}, // 2
	{-1, -1, 0xa, 1}, // 3
	{-1, -1, -1, -1}, // 4
	{-1, -1, -1, -1}, // 5	
	{-1, -1, -1, -1}, // 6
	{-1, -1, -1, -1}, // 7
	{-1, -1, -1, -1}, // 8
	{-1, -1, -1, -1}, // 9
	{3, -1, -1, 2},   // a
};

std::vector<char> command_directionpad(auto const& s)
{
	std::vector<char> rv;
	char start {0xa};
	for(auto c: s)
	{
		auto r = bfs(directionpad, start);
		auto rt{route(r, start, c)};
		rv.insert(rv.end(), rt.begin(), rt.end());
		rv.push_back(4);
		start = c;
	}
	return rv;
}

int64_t pt1(auto const& vv)
{
	timer t("pt1");
	for(auto& v: vv)
	{
		auto v1{command_keypad(v)};
//		print(v1);
		auto v2{command_directionpad(v1)};
//		print(v2);
		auto v3{command_directionpad(v2)};
//		print(v3);
		auto v4{command_directionpad(v3)};
		print(v4);
	}
	return 0;
}

int64_t pt2(auto const& vv)
{
	timer t("pt2");
	return 0;
}

int main()
{
	auto vv = get_input();
	std::cout << "pt1 = " << pt1(vv) << "\n";
	std::cout << "pt2 = " << pt2(vv) << "\n";
}
