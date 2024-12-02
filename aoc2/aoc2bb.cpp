#include <iostream>
#include <vector>
#include <string>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<uint64_t> rv;

	std::string ln;
	while(std::getline(std::cin, ln))
	{
		rv.push_back({});
		for (auto m : ctre::search_all<"(\\d+)">(ln))
		{
			rv.back() <<= 8;
			rv.back() |= m.to_number<unsigned char>();
		}
	}
	return rv;
}

bool safe(int64_t a)
{
	uint64_t zero_mask = 0xffffffffffffffff;
	uint64_t zero_test = 0xff00000000000000;
	while(!(a & zero_test))
	{
		zero_mask >>= 8;
		zero_test >>= 8;
	}
	zero_mask &= 0xffffffffffffff00;
	auto b = (a << 8) & zero_mask;
	a &= zero_mask;
	auto d = a > b ? a - b : b - a;
	while(zero_test != 0xff)
	{
		if(!(d & zero_test))
			return false;
		zero_test >>= 8;
	}
	return (d == (d & 0x03030303030303ff));
}

auto pt12(auto const& in)
{
//	timer t("p12");
	int cnt1{0};
	int cnt2{0};
	for (auto v : in)
	{
		if (safe(v))
		{
			++cnt1;
		}
		else
		{
			auto t = v >> 8;
			if (safe(t) ||
				safe((v & 0x00000000000000ff) | (t & 0xffffffffffffff00)) ||
				safe((v & 0x000000000000ffff) | (t & 0xffffffffffff0000)) ||
				safe((v & 0x0000000000ffffff) | (t & 0xffffffffff000000)) ||
				safe((v & 0x00000000ffffffff) | (t & 0xffffffff00000000)) ||
				safe((v & 0x000000ffffffffff) | (t & 0xffffff0000000000)) ||
				safe((v & 0x0000ffffffffffff) | (t & 0xffff000000000000)) ||
				safe( v & 0x00ffffffffffffff))
			{
				++cnt2;
			}
		}
	}
	return std::make_pair(cnt1, cnt1 + cnt2);
}

int main()
{
	auto in { get_input()};
	auto[p1, p2] = pt12(in);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
