#include <iostream>
#include <vector>
#include <string>
//#include <algorithm>

#include <bitset>

#include "ctre_inc.h"
#include "timer.h"

template<int MX, int MY> struct robot
{
	int x_;
	int y_;
	int vx_;
	int vy_;
	void step()
	{
		x_ += vx_;
		if( x_ < 0)
			x_ += MX;
		x_ %= MX;
		y_ += vy_;
		if(y_ < 0)
			y_ += MY;
		y_ %= MY;
	}
	bool q0() const
	{
		return x_ < MX / 2 && y_ < MY / 2;
	}
	bool q1() const
	{
		return x_ > MX / 2 && y_ < MY / 2;
	}
	bool q2() const
	{
		return x_ < MX / 2 && y_ > MY / 2;
	}
	bool q3() const
	{
		return x_ > MX / 2 && y_ > MY / 2;
	}
};

template<int MX, int MY> void render (std::vector<robot<MX, MY>> const& v)
{
	std::vector<char> rv( MX * MY, '.');
	for(auto r: v)
		rv[r.x_ + r.y_ * MX] = '*';
	for(int l { 0 }; l < MY; ++l)
	{
		std::string_view s(rv.data() + l * MX, MX);
		std::cout << s << "\n";
	}
}

auto get_input()
{
	std::vector<robot<101,103>> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		// p=6,3 v=-1,-3
		if(auto[m, x, y, vx, vy] = ctre::match<"p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)">(ln); m)
		{
			v.emplace_back(x.to_number<int>(), y.to_number<int>(), vx.to_number<int>(), vy.to_number<int>());
		}
	}

	return v;
}

int pt1(auto v)
{
	timer t("pt1");
	for(int n = 0; n < 100; ++n)
	{
		for(auto& r: v)
			r.step();
	}
	int64_t q0{0};
	int64_t q1{0};
	int64_t q2{0};
	int64_t q3{0};
	for(auto& r: v)
	{
		q0 += r.q0();
		q1 += r.q1();
		q2 += r.q2();
		q3 += r.q3();
	}
	return q1 * q2 * q3 * q0;
}

int64_t pt2(auto v)
{
	timer t("pt2");	
	std::bitset<103*101> bl;
	std::bitset<103*101> br;
	int cnt = 1;
	while(1)
	{
		bl.reset();
		br.reset();
		for(auto& r: v)
		{
			r.step();
			bl.set(r.x_ + r.y_ * 101);
			br.set(100 - r.x_ + r.y_ * 101 );
		}
		if((bl&br).count() > 78) // found this by trial and error
			break;
		++cnt;
	}
//	render(v);

	return cnt;
}

int main()
{
	auto in = get_input();
	if(in.size() < 500)
	{
		std::cout << "aoc14 hard coded to only process the actual input...\n";
		return 0;
	}
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
