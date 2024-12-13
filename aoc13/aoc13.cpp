#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"
#include "timer.h"

struct crane
{
	int64_t ax_;
	int64_t bx_;
	int64_t ay_;
	int64_t by_;
	int64_t tgt_x_;
	int64_t tgt_y_;
};

auto get_input()
{
	std::vector<crane> v;

	v.push_back({});
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			v.push_back({});
		if(auto[m, a, b] = ctre::match<"Button A: X\\+(\\d+),\\s*Y\\+(\\d+)">(ln); m)
		{
			v.back().ax_ = a.to_number<int64_t>();
			v.back().ay_ = b.to_number<int64_t>();
		}
		if(auto[m, a, b] = ctre::match<"Button B: X\\+(\\d+),\\s+Y\\+(\\d+)">(ln); m)
		{
			v.back().bx_ = a.to_number<int64_t>();
			v.back().by_ = b.to_number<int64_t>();
		}
		if(auto[m, a, b] = ctre::match<"Prize: X=(\\d+),\\s+Y=(\\d+)">(ln); m)
		{
			v.back().tgt_x_ = a.to_number<int64_t>();
			v.back().tgt_y_ = b.to_number<int64_t>();
		}
	}

	return v;
}

int64_t solve(crane const& c)
{
	int64_t A = (c.bx_ * c.tgt_y_ - c.by_ * c.tgt_x_) / (c.bx_ * c.ay_ - c.by_ * c.ax_);
	int64_t B = (c.tgt_x_ - A * c.ax_) / c.bx_;
	if(A * c.ax_ + B * c.bx_ == c.tgt_x_ &&
					A * c.ay_ + B * c.by_ == c.tgt_y_ )
		return 3 * A + B;
	return 0;
}

int64_t pt1(auto const& v)
{
	timer t("pt1");
	return std::reduce(v.begin(), v.end(), 0LL, [](auto s, auto& c){ return s + solve(c);});	
}

int64_t pt2(auto const& v)
{
	timer t("pt2");
	return std::reduce(v.begin(), v.end(), 0LL, [](auto s, auto c)
	{
		c.tgt_x_ += 10000000000000LL; 
		c.tgt_y_ += 10000000000000LL;
		return s + solve(c);
	});	
}

int main()
{
	auto in = get_input();
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
