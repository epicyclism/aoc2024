#include <iostream>
#include <vector>
#include <string>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

#include "timer.h"

auto get_input()
{
	std::string r;
	std::string ln;
	size_t stride{ 0 };
	while (std::getline(std::cin, ln))
	{
		if(stride == 0) stride = ln.size();
		r += ln;
	}
	return std::make_pair(stride, r);
}

struct pt
{
	int x_;
	int y_;
	constexpr pt(int x, int y) : x_{x}, y_{y}
	{}
};

//          N, E, S, W
int dir [] {0, 1, 2, 3};
int turn_right(int d)
{
	return d < 3 ? d + 1 : 0;
}

pt step(pt p, int d)
{
	switch(d)
	{
		case 0:
			return {p.x_, p.y_ - 1};
		case 1:
			return  {p.x_ + 1, p.y_};
		case 2:
			return {p.x_, p.y_ + 1};
		case 3:
			return {p.x_ - 1, p.y_};
	}
	return p; // can't happen.,..
}

bool valid(pt p, int stride)
{
	return p.x_ >= 0 && p.x_ < stride && p.y_ >= 0 && p.y_ < stride;
}

auto pt1(int stride, std::string s)
{
	timer t("pt1");
	stdex::mdspan md(s.data(), stride, stride);
	int st{int(s.find('^'))};
	pt b{st % stride, st / stride};
	int d { 0 };
	while(1)
	{
		auto n { step(b, d)};
		if(valid(n, stride))
		{
			if(md(n.y_, n.x_) != '#')
			{
				md(n.y_, n.x_) = ' ';
				b = n;
			}
			else
				d = turn_right(d);
		}
		else
			break;
	}
	return std::count(s.begin(), s.end(), ' ');
}

constexpr unsigned char df[] { 0x01, 0x02, 0x04, 0x08};

bool loop(int stride, std::string const& s)
{
	stdex::mdspan md(s.data(), stride, stride);
	int st{int(s.find('^'))};
	pt b{st % stride, st / stride};
	std::vector<unsigned char> flg(s.size());
	stdex::mdspan mdf(flg.data(), stride, stride);

	int d { 0 };
	while(1)
	{
		auto n { step(b, d)};
		if(valid(n, stride))
		{
			if(md(n.y_, n.x_) != '#')
			{
				if( mdf(n.y_, n.x_) & df[d])
					return true;
				mdf(n.y_, n.x_) |= df[d];
				b = n;
			}
			else
				d = turn_right(d);
		}
		else
			break;
	}
	return false;
}

auto pt2(int stride, std::string s)
{
	timer t("pt2");
	int cnt { 0 };
	for(auto& c: s)
	{
		if( c != '#' && c != '^')
		{
			c = '#';
			cnt += loop(stride, s);
			c = '.';
		}
	}
	return cnt;
}

int main()
{
	timer t("Overall");
	auto in {get_input()};
//	size_t stride { in.first};
	// assume square...
//    size_t height{ in.second.size() / in.first};
//	std::cout << stride << ", " << height << "\n";
	auto p1 = pt1(in.first, in.second);
	auto p2 = pt2(in.first, in.second);
	std::cout << "pt1 = " << p1 << "\n";
	std::cout << "pt2 = " << p2 << "\n";
}
