#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "ctre_inc.h"

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

void pt1(auto r, auto const& p)
{
	r.execute(p);
	for(auto c: r.out_)
		std::cout << +c << ",";
}

int64_t pt20(auto r, auto const& p)
{
//	int64_t sd{0x111111111111};
//	int64_t sd{0x349249249249};
//	int64_t sd{0x174bdf4};
	int64_t sd{0x40d4bdf4};
//	sd += 0xb613;
//	sd += 0xcf7598;
//	sd += 0xddd0;
//	sd += 0x68e170;
	for(int64_t n = 0; n < 0xffffff; ++n)
	{
		int64_t A = sd + (n << 27);
		r.A_ = A;
		r.execute(p);
		if(std::equal(r.out_.begin(), r.out_.begin() + 16, p.begin(), p.begin() + 16))
		{
			std::cout << std::hex << A << " " << n << " " ;
			for(auto c: r.out_)
				std::cout << +c << ",";
			std::cout << std::dec << "\n";
			return A;
		}
	}
	return 0;
}

int64_t pt2(auto r, auto const& p)
{
//	int64_t sd{0xbdf4};
	int64_t sd{0};
	int64_t shift { 0 };
	uint64_t cnt { 2 };
	while(cnt != 16)
	{
		auto tmp{ cnt };
		for (int64_t n{0}; n < 0xffff; ++n)
		{
			int64_t A = sd + (n << shift);
			r.A_ = A;
			r.execute(p);
			if(r.out_.size() >= cnt && std::equal(r.out_.begin(), r.out_.begin() + cnt, p.begin(), p.begin() + cnt))
			{
				std::cout << std::hex << A << " " << n << " " ;
				for(auto c: r.out_)
					std::cout << +c << ",";
				sd = A;
//				std::cout << "\n" << sd << std::dec << "\n";
				std::cout << "\n" << sd << "\n";
//				cnt = std::min( cnt + 1, r.out_.size() - 1);
				++cnt;
				shift += 3;
//				sd &= (1LL << shift) - 1;
				break;
			}
		}
		if (tmp == cnt)
		{
			break;
		}
	}
	return sd;
}
int main()
{
	auto [r, p] = get_input();
	std::cout << "A = " << r.A_ << ", B = " << r.B_ << ", C = " << r.C_ << "\n";
	for(auto i : p)
		std::cout << +i << ", ";
	std::cout << "\n";
	std::cout << "pt1  = " ; pt1(r, p) ; std::cout << "\n";
	std::cout << "pt2 = " << pt2(r, p) << "\n";
}

// pt2 0x21da40d4bdf4 == 37221274271220