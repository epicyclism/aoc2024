#include <iostream>
#include <vector>
#include <string>

#include "ctre_inc.h"
#include "mm_file.h"
#include "timer.h"


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

int main(int ac, char ** av)
{
	timer t("Overall");
	if (ac < 2)
		return -1;
	mem_map_file<char> mm{ av[1] };
	if (!mm)
		return -1;
	auto[p1, p2] = pt12(std::string_view{mm.ptr(), mm.length()});
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
