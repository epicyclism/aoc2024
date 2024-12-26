#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
//#include <functional>
#include <charconv>

#include "timer.h"

auto get_input()
{
	std::vector<uint64_t> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		uint64_t t{ 0 };
		std::from_chars(ln.data(), ln.data() + ln.size(), t);
		v.push_back(t);
	}
	
	return v;
}

inline uint64_t mix(uint64_t const u, uint64_t s)
{
	s ^= u;
	return s;
}

inline uint64_t prune(uint64_t s)
{
	return s % 16777216;
}

uint64_t evolve(uint64_t s)
{
	s = mix(s, s * 64);
	s = prune(s);
	s = mix(s, s / 32);
	s = prune(s);
	s = mix(s, s * 2048);
	s = prune(s);
	return s;
}

uint64_t evolve2(uint64_t s)
{
	s ^= s << 6;
	s &= 0xffffff;
	s ^= s >> 5;
	s &= 0xffffff;
	s ^= s * 2048ULL;
	s &= 0xffffff;
	return s;
}

int64_t pt1(auto const& v)
{
	timer t("pt1");
	return std::transform_reduce(v.begin(), v.end(), 0LL,
		std::plus<>(),
		 [](auto s)
			{
				for(int n = 0; n < 2000; ++n)
					s = evolve2(s);
				return s;
			});
}

inline uint32_t encode_sequence(char a, char b, char c, char d)
{
	uint32_t u = 0;
	u |= (a + 10);
	u <<= 5;
	u |= (b + 10);
	u <<= 5;
	u |= (c + 10);
	u <<= 5;
	u |= (d + 10);
	return u;
}

uint64_t pt2(auto const& v)
{
	timer t("pt2");
	uint64_t r = 0;
	std::vector<char> price;
	std::vector<char> diff;
	std::vector<char> vp(0xfffff);
	std::vector<int>  per_k(0xfffff);
//	auto k1 = encode_sequence(-2, 1, -1, 3);
	for(auto s: v)
	{
		price.clear();
		diff.clear();
		price.push_back(s % 10);
		for(int n = 0; n < 2000; ++n)
		{
			s = evolve2(s);
			price.push_back(s % 10);
		}
		diff.resize(price.size());
		std::adjacent_difference(price.begin(), price.end(), diff.begin());
		std::fill(vp.begin(), vp.end(), 0);
		for(int n = 1; n < diff.size() - 3; ++n)
		{
			auto k = encode_sequence(diff[n], diff[n + 1], diff[n + 2], diff[n + 3]);
			if(vp[k] == 0)
				vp[k] = price[n + 3];
		}
		std::transform(per_k.begin(), per_k.end(), vp.begin(), per_k.begin(), std::plus<>());
	}
	auto mx = std::max_element(per_k.begin(), per_k.end());
	return *mx;
}

int main()
{
	auto v = get_input();
	auto p1 = pt1(v);
	std::cout << "pt1 = " << p1 << "\n";
	auto p2 = pt2(v);
	std::cout << "pt2 = " << p2 << "\n";
}
