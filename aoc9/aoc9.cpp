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
		return;
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
	compact(ev), ev.end();
	return checksum(ev);
}

auto find_file(std::vector<int64_t>& v, std::vector<int64_t>::iterator re)
{
	while (*re == -1 && re != v.begin())
		--re;
	auto e{ re };
	auto id{ *re };
	while (*re == id && re != v.begin())
		--re;
	if(re != v.begin())
		return std::make_pair(re + 1, e + 1);
	return std::make_pair(re, e + 1);
}

auto find_space(std::vector<int64_t>& v, size_t sz, std::vector<int64_t>::iterator lim)
{
	auto bs{ v.begin() };
	while (bs < lim)
	{
		if (*bs == -1)
		{
			auto es{ bs + 1 };
			while (es != v.end())
			{
				if (*es != -1)
				{
					if ((es - bs) >= sz)
						return std::make_pair(bs, es);
					else
					{
						bs = es;
						break;
					}
				}
				++es;
			}
		}
		++bs;
	}
	return std::make_pair(bs, bs);
}

void compact2(std::vector<int64_t>& v)
{
	if (v.size() < 2)
		return ;
	auto bf{ v.end() };
	auto ef{ v.end() - 1};
	while (bf != v.begin())
	{
		std::tie(bf, ef) = find_file(v, bf - 1);
		auto [bs, es] = find_space(v, ef - bf, bf);
		if (bs != es)
		{
			for(auto b{bf}; b != ef; ++b)
			{
				std::swap(*b, *bs);
				++bs;
			}
		}
	}
}

int64_t pt2(std::string const& l)
{
	timer t ("pt2");
	auto ev{expand(l)};
	compact2(ev);
	return checksum(ev);
}

int main()
{
	auto l{get_input()};
	std::cout << "pt1 = " << pt1(l) << "\n";
	std::cout << "pt2 = " << pt2(l) << "\n";
}
