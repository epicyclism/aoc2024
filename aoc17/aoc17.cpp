#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "ctre_inc.h"
#include "timer.h"

struct regs
{
	int64_t A_;
	int64_t B_;
	int64_t C_;
	int64_t ip_;
	std::vector<char> out_;
	int64_t combo(char o)
	{
		switch(o)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			return o;
		case 4:
			return A_;
		case 5:
			return B_;
		case 6:
			return C_;
		case 7:
		default:
			return -1;
		}
	}
	void apply(char i, char o)
	{
		switch(i)
		{
			case 0:
				A_ = A_ / ( 1LL << combo(o));
				ip_ += 2;
				break;
			case 1:
				B_ ^= o;
				ip_ += 2;
				break;
			case 2:
				B_ = combo(o) & 0x07;
				ip_ += 2;
				break;
			case 3:
				if(A_ == 0)
					ip_ += 2;
				else
					ip_ = o;
				break;
			case 4:
				B_ ^= C_;
				ip_ += 2;
				break;
			case 5:
				out_.emplace_back(char(combo(o) % 8));
				ip_ += 2;
				break;
			case 6:
				B_ = A_ / ( 1LL << combo(o));
				ip_ += 2;
				break;
			case 7:
				C_ = A_ / ( 1LL << combo(o));
				ip_ += 2;
				break;
			default:
				break;
		}
	}
	void execute(std::vector<char> const& p)
	{
		ip_ = 0;
		out_.clear();
		while(ip_ < p.size() - 1)
		{
			apply(p[ip_], p[ip_ + 1]);
		}
	}
};

auto get_input()
{
	regs r;
	std::vector<char> program;
	std::string ln;

	std::getline(std::cin, ln);
	if(auto[m, v] = ctre::match<"Register [A|B|C]: (\\d+)">(ln); m)
		r.A_ = v.to_number<int64_t>();
	std::getline(std::cin, ln);
	if(auto[m, v] = ctre::match<"Register [A|B|C]: (\\d+)">(ln); m)
		r.B_ = v.to_number<int64_t>();
	std::getline(std::cin, ln);
	if(auto[m, v] = ctre::match<"Register [A|B|C]: (\\d+)">(ln); m)
		r.C_ = v.to_number<int64_t>();
	std::getline(std::cin, ln);
	std::getline(std::cin, ln);
	for (auto m : ctre::search_all<"(\\d+)">(ln))
		program.emplace_back(m.to_number<char>());

	return std::make_pair(r, program);
}

std::string pt1(auto r, auto const& p)
{
	timer t("pt1");
	std::string s;
	s.reserve(31);
	r.execute(p);
	for(auto c: r.out_)
	{
		if(!s.empty())
			s.push_back(',');
		s.push_back(c + '0');
	}
	return s;
}

bool eval(int64_t k, auto& r, auto const& p, size_t cmp, int64_t& a)
{
	for (int64_t n{ 0 }; n < 64; ++n)
	{
		r.A_ = n + k;
		r.execute(p);
		if(r.out_.size() >= p.size() - cmp && 
			std::equal(p.begin() + cmp, p.end(), r.out_.begin()))
		{
			if(cmp == 0)
			{
				a = n + k;
				return true;
			}
			if (eval((n + k) << 6, r, p, cmp - 2, a))
				return true;
		}
	}
	return false;
}

int64_t pt2(auto r, auto const& p)
{
	timer t("pt2");
	int64_t res{ 0 };
	eval(0, r, p, p.size() - 2, res);
	return res;
}

int main()
{
	auto [r, p] = get_input();

	std::cout << "pt1 = " << pt1(r, p) << "\n";
	std::cout << "pt2 = " << pt2(r, p) << "\n";
}

// pt2 0x21da40d4bdf4 == 37221274271220