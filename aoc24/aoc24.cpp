#include <iostream>
#include <vector>
#include <array>
#include <map>
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
std::string_view to_sv(op o)
{
	switch(o)
	{
		case AND:
			return "AND";
		case OR:
			return "OR";
		case XOR:
			return "XOR";
		default:
			return "TILT";
	}
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
		bool A = vars[A_];
		bool B = vars[B_];
		acted_ = true;
		switch(op_)
		{
			case AND:
				vars[o_] = A & B;
				break;
			case OR:
				vars[o_] = A | B;
				break;
			case XOR:
				vars[o_] = A ^ B;
				break;
			default:
				break;
		}
	}
};

auto get_input()
{
	std::map<nm, bool> vars;
	std::vector<gate> nw;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			continue;
		if(auto[m, a, b] = ctre::match<"(\\w+): (\\d)">(ln); m)
		{
			vars[a.to_string()] = b.to_view() == "1";
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
			r |= (*i).second ? 1ULL : 0ULL;
		}
	}
	return r;
}

void reset(auto& vars, auto& nw)
{
	for(auto& v: vars)
		v.second = false;
	for(auto& g: nw)
		g.acted_ = false;
}

void swap(gate& g1, gate& g2)
{
	std::swap(g1.o_, g2.o_);
}

uint64_t process(uint64_t xv, uint64_t yv, auto& vars, auto& nw)
{
	uint64_t xb = 1;
	uint64_t yb = 1;
	reset(vars, nw);
	for(auto i = vars.begin(); i != vars.end(); ++i)
	{
		if((*i).first[0] == 'x')
		{
			(*i).second = (xv & xb) ? true : false;
			xb <<= 1;
		}
		if((*i).first[0] == 'y')
		{
			(*i).second = (yv & yb) ? true : false;
			yb <<= 1;
		}
	}
	while(!std::all_of(nw.begin(), nw.end(), [](auto const& g){ return g.acted_;}))
		for(auto& g: nw)
			g.act(vars);

	uint64_t r = 0;
	uint64_t b = 1LL;
	for(auto i = vars.begin(); i != vars.end(); ++i)
	{
		if((*i).first[0] == 'z')
		{
			if((*i).second)
				r |= b;
			b <<= 1;
		}
	}
	return r;
}

std::ostream& operator<<(std::ostream& o, gate const& g)
{
	o << g.A_ << " " << to_sv(g.op_ ) << " " << g.B_ << " -> " << g.o_ ;
	return o;
}

void trace(nm in, auto const& nw)
{
	std::cout << in << " : ";
	for(auto& g: nw)
		if(g.A_ == in || g.B_ == in)
		{
			std::cout << g << " ";
			trace( g.o_,nw);
		}
	std::cout << "\n";
}	

constexpr uint64_t val = 0xfffffffffff;
constexpr uint64_t res = 0x1ffffffffffe;
constexpr uint64_t lim = 0x100000000000;
int64_t pt2(auto vars, auto nw)
{
	timer t("pt2");
	uint64_t b = 1ULL;
	std::vector<std::pair<int, int>> swps;
	uint64_t v = process(val, 1, vars, nw);
	while( v != res)
	{
	out0:
		while((( v & b ) ^ (res & b)) == 0)
		{
			if(b == lim)
			{
				std::cout << "tilt\n";
				goto out;
			}
			b <<= 1;
		}
		for(int n = 0; n < nw.size() - 1; ++n)
		{
			for (int m = n + 1; m < nw.size(); ++m)
			{
				swap(nw[n], nw[m]);
				v = process(val, 1, vars, nw);
				if (((v & b) ^ (res & b)) == 0)
				{
					if(n < m)
						std::cout << nw[n].o_ << " " << nw[m].o_ << "\n";
					else
						std::cout << nw[m].o_ << " " << nw[n].o_ << "!\n";
					swps.emplace_back(n, m);
//					goto out0;
				}
				swap(nw[n], nw[m]);
			}
		}
		goto out;

	}
out:
	for(auto p: swps)
		std::cout << nw[p.first].o_ << " " << nw[p.second].o_ << "\n";

	return 0;
}

int main()
{
	auto [v, n] = get_input();
	std::cout << "var count = " << v.size() << ", gate count = " << n.size() << "\n";
	auto p1 = pt1(v, n);
	std::cout << "pt1 = " << p1 << "\n";
	auto p2 = pt2(v, n);
	std::cout << "pt2 = " << p2 << "\n";
}
