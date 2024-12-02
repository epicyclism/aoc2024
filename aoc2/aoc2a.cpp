#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int64_t> rv;

	std::string ln;
	while(std::getline(std::cin, ln))
	{
		rv.push_back({});
		int cnt{ 0 };
		for (auto m : ctre::search_all<"(\\d+)">(ln))
		{
			rv.back() <<= 8;
			rv.back() |= m.to_number<unsigned char>() + 7;
			++cnt;
		}
		while (cnt < 8)
		{
			int a = rv.back() & 0xff;
			int d = a - ((rv.back() >> 8) & 0xff);
			rv.back() <<= 8;
			rv.back() |= (a + d) & 0xff;
			++cnt;
		}
	}
	return rv;
}

bool pass(int64_t a)
{
	auto b = a << 8;
	auto d = a - b;
	if (b > a)
	{
		b |= 0x7f;
		d = b - a;
	}
	return (d == (d & 0x03030303030303ff)) &&
				(d & 0xff00000000000000)&&
				(d & 0x00ff000000000000)&&
				(d & 0x0000ff0000000000)&&
				(d & 0x000000ff00000000)&&
				(d & 0x00000000ff000000)&&
				(d & 0x0000000000ff0000)&&
				(d & 0x000000000000ff00);
}

bool pass7(int64_t a)
{
	auto b = a << 8;
	auto d = a - b;
	if (b > a)
	{
		b |= 0x7f7f;
		d = b - a;
	}
	return (d == (d & 0x030303030303ffff)) &&
		(d & 0xff00000000000000) &&
		(d & 0x00ff000000000000) &&
		(d & 0x0000ff0000000000) &&
		(d & 0x000000ff00000000) &&
		(d & 0x00000000ff000000) &&
		(d & 0x0000000000ff0000) ;
}

auto pt12(auto const& in)
{
	timer t("p12");
	int cnt1{0};
	int cnt2{0};
	for (auto v : in)
	{
		if (pass(v))
			++cnt1;
		else
		{
			auto t = v << 8;
			if (pass7(t) ||
				pass7((v & 0xff00000000000000) | (t & 0x00ffffffffffffff)) ||
				pass7((v & 0xffff000000000000) | (t & 0x0000ffffffffffff)) ||
				pass7((v & 0xffffff0000000000) | (t & 0x000000ffffffffff)) ||
				pass7((v & 0xffffffff00000000) | (t & 0x00000000ffffffff)) ||
				pass7((v & 0xffffffffff000000) | (t & 0x0000000000ffffff)) ||
				pass7((v & 0xffffffffffff0000) | (t & 0x000000000000ffff)) ||
				pass7(v & 0xffffffffffffff00))
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
