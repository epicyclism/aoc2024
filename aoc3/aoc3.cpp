#include <iostream>
#include <vector>
#include <string>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::string r;
	std::string ln;
	while(std::getline(std::cin, ln))
		r += ln;
	return r;
}

auto pt12(auto const& in)
{
	timer t("p12");
	bool do_{true};
	int64_t p1{0};
	int64_t p2{0};
	for (auto [m, a, b, d1, d0] : ctre::search_all<"mul\\((\\d{1,3}),(\\d{1,3})\\)|(do\\(\\))|(don't\\(\\))">(in))
	{
		if(d1)
			do_ = true;
		else if(d0)
			do_ = false;
		else
		{
			auto p = a.template to_number<int>() * b.template to_number<int>();
			p1 += p;
			if(do_)
				p2 += p;
		}
	}

	return std::make_pair(p1, p2);
}

int main()
{
	auto in {get_input()};
	auto[p1, p2] = pt12(in);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
