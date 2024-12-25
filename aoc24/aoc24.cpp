#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <string>
#include <algorithm>

#include "ctre_inc.h"
#include "timer.h"

using nm = std::string;
enum op { AND, OR, XOR, INVAL};
op from_sv(std::string_view sv)
{
	if(sv == "AND")
		return AND;
	if(sv == "OR")
		return OR;
	if(sv == "XOR")
		return XOR;
	return INVAL;
}

struct gate
{
	nm A_;
	nm B_;
	nm o_;
	op op_;
	bool acted_ = false;
	void act(auto& vars )
	{
		if(acted_)
			return;
		if(!(vars.contains(A_) && vars.contains(B_)))
			return;
		bool A = vars[A_].second;
		bool B = vars[B_].second;
		acted_ = true;
		switch(op_)
		{
			case AND:
				vars[o_].second = A & B;
				break;
			case OR:
				vars[o_].second = A | B;
				break;
			case XOR:
				vars[o_].second = A ^ B;
				break;
			default:
				break;
		}
	}
	void act2(auto& vars )
	{
		if(acted_)
			return;
		if(!(vars[A_].first && vars[B_].first))
			return;
		bool A = vars[A_].second;
		vars[A_].first = true;
		bool B = vars[B_].second;
		vars[B_].first = true;
		acted_ = true;
		switch(op_)
		{
			case AND:
				vars[o_].second = A & B;
				break;
			case OR:
				vars[o_].second = A | B;
				break;
			case XOR:
				vars[o_].second = A ^ B;
				break;
			default:
				break;
		}
	}
};

auto get_input()
{
	std::map<nm, std::pair<bool,bool>> vars;
	std::vector<gate> nw;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			continue;
		if(auto[m, a, b] = ctre::match<"(\\w+): (\\d)">(ln); m)
		{
			vars[a.to_string()] = {false, b.to_view() == "1"};
		}
		if(auto[m, x, l, y, o] = ctre::match<"(\\w+) ([A-Z]+) (\\w+) -> (\\w+)">(ln); m)
		{
			nw.emplace_back(x.to_string(), y.to_string(), o.to_string(), from_sv(l.to_view()));
		}
	}

	return std::make_pair(vars, nw);
}

uint64_t pt1(auto vars, auto nw)
{
	timer t("pt1");
	while(!std::all_of(nw.begin(), nw.end(), [](auto const& g){ return g.acted_;}))
	{
		for(auto& g: nw)
		{
			g.act(vars);
		}
	}
	uint64_t r = 0;
	for(auto i = vars.rbegin(); i != vars.rend(); ++i)
	{
		if((*i).first[0] == 'z')
		{
			r <<= 1;
			r |= (*i).second.second ? 1ULL : 0ULL;
		}
	}
	return r;
}

bool is_input_of(auto& nw, op opp, nm& n)
{
	for(auto& g: nw)
	{
		if( g.op_ == opp && (g.A_ == n || g.B_ == n))
			return true;
	}
	return false;
}

std::string pt2(auto vars, auto nw)
{
	timer t("pt2");
	std::set<std::string> bad;
	for(auto& g: nw)
	{
		if( g.o_[0] == 'z' && g.op_ != XOR)
			if(!(g.o_[1] == '4' && g.o_[2] == '5'))
				bad.insert(g.o_);
	}
	for(auto& g: nw)
	{
		if(g.o_[0] != 'z' && !((g.A_[0] == 'x' && g.B_[0] == 'y') || (g.A_[0] == 'y' && g.B_[0] == 'x')))
			if(g.op_ == XOR)
				bad.insert(g.o_);
	}
	for(auto& g: nw)
	{
		if( g.op_ == XOR && ((g.A_[0] == 'x' && g.B_[0] == 'y') || (g.A_[0] == 'y' && g.B_[0] == 'x')))
			if(!is_input_of(nw, XOR, g.o_) && !((g.A_ == "x00" && g.B_ == "y00") || (g.A_ == "y00" && g.B_ == "x00")))
				bad.insert(g.o_);
	}
	for(auto& g: nw)
	{
		if( g.op_ == AND  && !((g.A_ == "x00" && g.B_ == "y00") || (g.A_ == "y00" && g.B_ == "x00")))
			if(! is_input_of(nw, OR, g.o_))
				bad.insert(g.o_);
	}
	std::string s;
	for(auto& p : bad)
	{
		if(!s.empty())
			s += ',';
		s += p;
	}
	return s;
}

int main()
{
	auto [v, n] = get_input();
	auto p1 = pt1(v, n);
	std::cout << "pt1 = " << p1 << "\n";
	auto p2 = pt2(v, n);
	std::cout << "pt2 = " << p2 << "\n";
}
