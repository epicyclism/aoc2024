#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

std::vector<int64_t> expand(std::string const& s)
{
	std::vector<int64_t> r;
	int64_t id { 0 };
	bool f { true};
	for(auto i: s)
	{
		if(f)
		{
			r.insert(r.end(), i - '0', id);
			++id;
			f = false;
		}
		else
		{
			r.insert(r.end(), i - '0', -1);
			f = true;
		}
	}
	return r;
}

auto compact(std::vector<int64_t>& v)
{
	if(v.size () < 2)
		return v.end();
	auto b { v.begin()};
	auto e { v.end() - 1};
	while (b < e)
	{
		if(*b != -1)
			++b;
		else
		if(*e == -1)
			--e;
		else
		{
			std::swap(*b, *e);
			++b;
			--e;
		}
	}
	while(*b == -1)
		--b;
//	for(auto x: v)
//		std::cout << x << "\n";
	return b + 1;
}

int64_t checksum(std::vector<int64_t> const& v)
{
	int64_t cs{0};
	for(size_t n { 0 }; n < v.size(); ++n)
		if(v[n] != -1)
			cs += n * v[n];
	return cs;
}

int64_t pt1(std::string const& l)
{
	timer t ("pt1");
	auto ev{expand(l)};
	ev.erase(compact(ev), ev.end());
	return checksum(ev);
}

int64_t pt2(std::string const& l)
{
	timer t ("pt2");
	auto ev{expand(l)};
	ev.erase(compact(ev), ev.end());
	return checksum(ev);
}

int main()
{
	auto l{get_input()};
	std::cout << "pt1 = " << pt1(l) << "\n";
	std::cout << "pt2 = " << pt2(l) << "\n";
}
