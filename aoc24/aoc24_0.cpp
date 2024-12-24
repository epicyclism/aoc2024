#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <bit>

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

int64_t pt1(auto vars, auto nw)
{
	timer t("pt1");
	while(!std::all_of(nw.begin(), nw.end(), [](auto const& g){ return g.acted_;}))
	{
		for(auto& g: nw)
		{
			g.act(vars);
		}
	}
	int64_t r = 0;
	for(auto i = vars.rbegin(); i != vars.rend(); ++i)
	{
		if((*i).first[0] == 'z')
		{
			r <<= 1;
			r |= (*i).second;
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

bool test_bit0(int b, auto& vars, auto& nw)
{
	auto xy = 1LL << b;
	return process(0, xy, vars, nw) == xy;
}

bool test_bit1(int b, auto& vars, auto& nw)
{
	auto xy = 1LL << b;
	return process(xy, 0, vars, nw) == xy;
}
bool test_bit2(int b, auto& vars, auto& nw)
{
	auto xy = 1LL << b;
	return process(xy, xy, vars, nw) == xy << 1;
}
void print_netlist(auto const& nw)
{
	int gn = 1;
	std::cout << "*\n";
	for(auto& g: nw)
	{
		std::cout << "G" << gn << " " << g.A_ << " " << g.B_ << " 0 0 0 0 " <<
				 g.o_ << " 0 " << to_sv (g.op_) << "\n";
		++gn;
	}
	std::cout << ".anno\n.end\n";
}

int64_t cnt ;
int64_t tl  ;
void swap_search(auto& vars, auto& nw, std::vector<std::pair<int, int>>& swps)
{
	if( swps.size() == 4)
	{
		++cnt;
		if ( cnt == 1000000)
		{
			std::cout << tl << "\n";
			tl += cnt;
			cnt = 0;
		}
		return;
	}
	for(int n = 0; n < nw.size() - 1; ++n)
	{
		for(int m = 0; m < nw.size(); ++m)
		{
	//		if(nw[n].o_[0] == 'x' || nw[n].o_[0] == 'y' || nw[n].o_[0] == 'z')
	//		continue;
			if(std::find_if(swps.begin(), swps.end(), [=](auto& pr){ return pr.first == n || pr.second == n || pr.first == m || pr.second == m;}) == swps.end())
			{
				swps.emplace_back(n, m);
				swap_search(vars, nw, swps);
				swps.pop_back();
			}
		}
	}
}

void swap_search0(auto& vars, auto& nw, std::vector<std::pair<int, int>>& swps, int pc)
{
	if( swps.size() == 4)
	{
		++cnt;
		if ( cnt == 1000)
		{
			std::cout << tl;
			tl += cnt;
			cnt = 0;
		}
		if (pc == 44)
		{
			for(auto& p: swps)
				std::cout << nw[p.first].o_ << " - " << nw[p.second].o_ << "\n";
			std::cout << "\n";
		}
//		std::cout << pc << "\n";
		return;
	}
	for(int n = 0; n < nw.size() - 1; ++n)
	{
		if(nw[n].o_[0] == 'x' || nw[n].o_[0] == 'y' || nw[n].o_[0] == 'z')
			continue;
		if(std::find_if(swps.begin(), swps.end(), [=](auto& pr){ return pr.first == n || pr.second == n;}) == swps.end())
		{
//			std::cout << n << "\n";
				for(int m = 1; m < nw.size(); ++m)
				{
					if(nw[n].o_[0] == 'x' || nw[n].o_[0] == 'y' || nw[n].o_[0] == 'z')
						continue;
if(std::find_if(swps.begin(), swps.end(), [=](auto& pr){ return pr.first == m || pr.second == m;}) == swps.end())
					{
						swap(nw[n], nw[m]);
						auto r = process(0xfffffffffff, 1, vars, nw) ;
						if(std::popcount(r) <= pc)
						{
							swps.emplace_back(n, m);
							swap_search0(vars, nw, swps, std::popcount(r));
							swps.pop_back();
						}
						swap(nw[n], nw[m]);
					}
				}
		}
	}
}

int64_t pt2(auto vars, auto nw)
{
#if 1
	timer t("pt2");
#if 0
	auto b = 1LL;
	for(int n = 0; n < 45; ++n)
	{
		std::cout << b << " = " << process(0, b, vars, nw) << "\n";
		b <<= 1;
	}

	std::cout << std::hex << process(0xfffffffffff, 0xfffffffffff, vars, nw) << "\n";
	for(int n = 0; n < 45; ++n)
	{
		std::cout << "bit " << n <<  (test_bit0(n, vars, nw) ? " 0 good\n" : " 0 bad\n");
		std::cout << "bit " << n <<  (test_bit1(n, vars, nw) ? " 1 good\n" : " 1 bad\n");
		std::cout << "bit " << n <<  (test_bit2(n, vars, nw) ? " 2 good\n" : " 2 bad\n");
	}
#endif

#if 0
	trace("x44", nw);
	trace("x00", nw);
	trace("y00", nw);
	trace("z00", nw);
	trace("x01", nw);
	trace("y01", nw);
	trace("z01", nw);
	trace("x02", nw);
	trace("y02", nw);
	trace("z02", nw);
	trace("x03", nw);
	trace("y03", nw);
	trace("z03", nw);
	trace("x04", nw);
	trace("y04", nw);
	trace("z04", nw);
	trace("x05", nw);
	trace("y05", nw);
	trace("z05", nw);
	trace("x06", nw);
	trace("y06", nw);
	trace("z06", nw);
	trace("x07", nw);
	trace("y07", nw);
	trace("z07", nw);
#endif
#endif
	cnt = 0;
	tl = 0;	
	std::vector<std::pair<int, int>> swps;
	swap_search(vars, nw, swps);
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
