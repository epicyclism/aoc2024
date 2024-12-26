#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"
#include "timer.h"

using key_lock_t = std::array<char, 5>;

auto get_input()
{
	std::vector<key_lock_t> locks;
	std::vector<key_lock_t> keys;
	std::string ln;
	key_lock_t tmp;
	tmp.fill(-1);
	int state = 0; // 1 == key, 2 == lock
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
		{
			switch(state)
			{
				default:
				case 0:
					break;
				case 1:
					keys.push_back(tmp);
					state = 0;
					break;
				case 2:
					locks.push_back(tmp);
					state = 0;
					break;
			}
		}
		switch(state)
		{
			default:
			case 0:
				if(!ln.empty())
				{
					if(ln[0] == '#')
						state = 2;
					else
						state = 1;
					tmp.fill(-1);
				}
			case 1:
			case 2:
				for(size_t n = 0; n < tmp.size(); ++n)
					if(ln[n] == '#')
						++tmp[n];
				break;
		}
	}
	if(state == 1)
		keys.push_back(tmp);
	if(state == 2)
		locks.push_back(tmp);
	return std::make_pair(locks, keys);
}


bool fits(key_lock_t const& l, key_lock_t const& k)
{
	for(size_t n = 0; n < l.size(); ++n)
		if( l[n] + k[n] > 5)
			return false;
	return true;
}

auto pt1(auto const& locks, auto const& keys)
{
	timer t("pt1");
	int cnt = 0;
	for(auto& l : locks)
		for(auto& k : keys)
			cnt += fits(l, k);
	return cnt;
}

int main()
{
	auto[locks, keys] = get_input();
	auto p1 = pt1(locks, keys);
	std::cout << "pt1 = " << p1 << "\n";
}
