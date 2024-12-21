#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>
#include <span>

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

std::vector<std::pair<int, std::vector<char>>> kp_crib1
{
	{
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
			return kpc.second;
		}
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
		int64_t sz{0};
		auto it{v1.begin()};
		auto it2{it};
		while(it2 != v1.end())
		{
			while(*it2 != 0xa)
				++it2;
			++it2;
			auto sp = std::span(it, it2);
			auto v2 = command_directionpad(sp);
			for(int n = 1; n < 25; ++n)
				v2 = command_directionpad(v2);
			sz += v2.size();
			it = it2;
			std::cout << "   " << sz << "\n"
;		}
		std::cout << sz << "\n";
		r += sz * id;
	}
	return r;
}

int main()
{
	auto vv = get_input();

	auto p1{pt1(vv)};
	std::cout << "pt1 = " << p1 << "\n";

	auto p2{pt2(vv)};
	std::cout << "pt2 = " << p2 << "\n";
}

//
// 133054 too high
// 132350 too high
// 132018 too high
// 130490 ??
// 128962 ??

// 336005649663463 too high
// 336471742129638
// 131006741852991 too low
// 863544533437568
// 336224844349270 too high