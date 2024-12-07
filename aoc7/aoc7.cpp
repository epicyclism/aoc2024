#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"
#include "timer.h"

struct equation
{
	uint64_t val_;
	std::vector<uint64_t> pts_;
};

auto get_input()
{
	std::vector<equation> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		equation e;
		bool p{false};
		for(auto v: ctre::search_all<"(\\d+)">(ln))
		{
			if(p)
				e.pts_.emplace_back(v.to_number<uint64_t>());
			else
				e.val_ = v.to_number<uint64_t>();
			p = true;
		}
		rv.emplace_back(e);
	}

	return rv;
}

int64_t eval(std::vector<uint64_t> const& val, uint64_t ops)
{
	return std::accumulate(val.begin() + 1, val.end(), val.front(), [&](auto a, auto& b)
		{
			if(ops & 1)
				a *= b;
			else
				a += b;
			ops >>= 1;
			return a;
		});
}

uint64_t ashift(uint64_t a, uint64_t b)
{
	while(b)
	{
		a*=10;
		b/=10;
	}
	return a;
}

uint64_t eval2(std::vector<uint64_t> const& val, uint64_t ops)
{
	return std::accumulate(val.begin() + 1, val.end(), val.front(), [&](auto a, auto& b)
		{
			switch(ops & 0x03)
			{
				default:
				case 0:
					a += b;
					break;
				case 1:
					a *= b;
					break;
				case 2:
					a = ashift(a, b) + b;
					break;
			}
			ops >>= 2;
			return a;
		});
}

bool can_be(equation const& e)
{
	uint64_t lim { (1ULL << (e.pts_.size() - 1)) - 1};
	for(uint64_t op { 0 }; op <= lim; ++op)
		if(eval(e.pts_, op) == e.val_)
			return true;
	return false;
}

bool valid(uint64_t op)
{
	while(op)
	{
		if((op & 0x03) == 0x03)
			return false;
		op >>= 2;
	}
	return true;
}

bool can_be2(equation const& e)
{
	uint64_t lim { 1ULL << ((e.pts_.size() - 1) * 2)};
	--lim;
	for(uint64_t op { 0 }; op <= lim; ++op)
	{
		if( valid(op) && eval2(e.pts_, op) == e.val_)
			return true;
	}
	return false;
}

int64_t pt1(auto const& in)
{
	timer t ("p1");
	uint64_t r{0};
	for(auto& e: in)
		if(can_be(e))
			r += e.val_;
	return r;
}

int64_t pt2(auto const& in)
{
	timer t ("p2");
	uint64_t r{0};
	for(auto& e: in)
		if(can_be2(e))
			r += e.val_;
	return r;
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
