#include <iostream>
#include <vector>
#include <map>
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
		for(auto v: ctre::search_all<"(\\d+)">(ln))
				rv.emplace_back(v.to_number<uint64_t>());
	}

	return rv;
}

int count_digits(int64_t v)
{
    int64_t p10{1};
    int digits{0};
    while(v)
    {
        v /= 10;
        ++digits;
    }
    return digits;
}

int64_t power10(int p)
{
    int64_t v{1};
    while(p)
    {
        v *= 10;
        --p;
    }
    return v;
}

std::pair<int64_t, int64_t> pt12(auto in)
{
    timer t("pt12");
    std::map<int64_t, int64_t> m;
    for(auto v: in)
        m[v]++;
    int64_t p1;
    for (int n {0}; n < 75; ++n)
    {
        if(n == 25)
            p1 = std::accumulate(m.begin(), m.end(), 0LL, [](auto s, auto& me){ return me.second + s;});
        std::map<int64_t, int64_t> tm;
        for(auto& me: m)
        {
            auto v {me.first};
            auto cnt{ me.second};
            if(v == 0)
                tm[1] += cnt;
            else
            {
                auto d{count_digits(v)};
                if(!(d&1))
                {
                    int64_t p10{power10(d/2)};
                    tm[v / p10] += cnt;
                    tm[v % p10] += cnt;
                }
                else
                    tm[v * 2024] += cnt;
            }
        }
        m.swap(tm);
    }
 	return {p1, std::accumulate(m.begin(), m.end(), 0LL, [](auto s, auto& me){ return me.second + s;})};
}

int main()
{
	auto in{get_input()};
    auto[p1, p2] = pt12(in);
    std::cout << p1 << "\n" << p2 << "\n";
}