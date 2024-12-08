#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "timer.h"

using pt = std::pair<int, int>;
struct a_t
{
	pt   p_;
	char t_;
	a_t(int x, int y, char c) : p_(x, y), t_(c)
	{}
};

auto get_input()
{
	std::vector<a_t> rv;
	std::string ln;
	int y { 0 };
	size_t s { 0 };
	while(std::getline(std::cin, ln))
	{
		if(s == 0)
			s = ln.size();
		for(int x { 0 }; x < ln.size(); ++x)
			if(ln[x] != '.')
			rv.emplace_back(x, y, ln[x]);
		++y;
	}
	return std::make_pair(s, rv);
}

bool valid(pt const& a, size_t s)
{
	return a.first >=0 && a.first < s &&
			a.second >= 0 && a.second < s;
}

std::pair<pt, pt> anti(pt const& a1, pt const& a2)
{
	int d_x { a1.first - a2.first};
	int d_y { a1.second - a2.second};
	return std::make_pair(pt(a1.first + d_x, a1.second + d_y),
								pt(a2.first - d_x, a2.second - d_y) );
}

uint64_t pt1(auto const& in)
{
	timer t ("p1");
	size_t st{ in.first};
	auto& va{in.second};
	std::vector<pt> van;
	for(size_t i { 0 }; i < in.second.size() - 1; ++i)
		for(size_t j { i + 1}; j < in.second.size(); ++j)
		{
			if(va[i].t_ == va[j].t_)
			{
				auto[an1, an2] = anti(va[i].p_, va[j].p_);
				if(valid(an1, st))
					van.emplace_back(an1);
				if(valid(an2, st))
					van.emplace_back(an2);
			}
		}
	std::sort(van.begin(), van.end());
	van.erase(std::unique(van.begin(), van.end()), van.end());
	return van.size();
}

void project(pt a1, pt a2, size_t st, std::vector<pt>& van)
{
	int d_x { a2.first - a1.first};
	int d_y { a2.second - a1.second};
	int gd = std::gcd(d_x, d_y);
	d_x /= gd;
	d_y /= gd;
	while(valid(a1, st))
	{
		a1.first -= d_x;
		a1.second -= d_y;
	}
	a1.first += d_x;
	a1.second += d_y;
	while(valid(a1, st))
	{
		van.emplace_back(a1);
		a1.first += d_x;
		a1.second += d_y;
	}
}

int64_t pt2(auto const& in)
{
	timer t ("p2");
	size_t st{ in.first};
	auto& va{in.second};
	std::vector<pt> van;
	for(size_t i { 0 }; i < in.second.size() - 1; ++i)
		for(size_t j { i + 1}; j < in.second.size(); ++j)
		{
			if(va[i].t_ == va[j].t_)
			{
				project(va[i].p_, va[j].p_, st, van);
			}
		}
	std::sort(van.begin(), van.end());
	van.erase(std::unique(van.begin(), van.end()), van.end());
	return van.size ();
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
