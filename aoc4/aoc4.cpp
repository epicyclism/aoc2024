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
using vane = pt[3];
constexpr vane E  = {{1, 0}, {2, 0}, {3, 0}};
constexpr vane SE = {{1, 1}, {2, 2}, {3, 3}};
constexpr vane S  = {{0, 1}, {0, 2}, {0, 3}};
constexpr vane SW = {{-1, 1}, {-2, 2}, {-3, 3}};
constexpr vane W  = {{-1, 0}, {-2, 0}, {-3, 0}};
constexpr vane NW = {{-1, -1}, {-2, -2}, {-3,-3}};
constexpr vane N  = {{0, -1}, {0, -2}, {0, -3}};
constexpr vane NE = {{1, -1}, {2, -2}, {3, -3}};

bool find(vane const& v, auto& md, pt p)
{
	return md(p.y_ + v[0].y_, p.x_ + v[0].x_) == 'M' &&
				md(p.y_ + v[1].y_, p.x_ + v[1].x_) == 'A' &&
				md(p.y_ + v[2].y_, p.x_ + v[2].x_) == 'S';
}

auto pt1(auto in)
{
	auto stride = in.extent(0);
	auto height = in.extent(1);
	int cnt { 0 };
	for(auto x { 0}; x < stride; ++x)
		for(auto y { 0 }; y < height; ++y)
		{
			if(in(y, x) == 'X')
			{
				bool e = x < stride - 3;
				bool s = y < height - 3;
				bool w = x > 2;
				bool n = y > 2;
				if(e)
					cnt += find(E, in, {x, y});
				if(e && s)
					cnt += find(SE, in, {x, y});
				if(s)
					cnt += find(S, in, {x, y});
				if(s && w)
					cnt += find(SW, in, {x, y});
				if(w)
					cnt += find(W, in, {x, y});
				if(w && n)
					cnt += find(NW, in, {x, y});
				if(n)
					cnt += find(N, in, {x, y});
				if(n && e)
					cnt += find(NE, in, {x, y});
			}
		}
	return cnt;
}

using vanex = pt[2];

constexpr vanex SEx = {{1, 1}, {-1, -1}};
constexpr vanex SWx = {{-1, 1}, {1, -1}};
constexpr vanex NWx = {{-1, -1}, {1, 1}};
constexpr vanex NEx = {{1,-1}, {-1, 1}};

bool findx(vanex const& v, auto& md, pt p)
{
	return md(p.y_ + v[0].y_, p.x_ + v[0].x_) == 'M' &&
				md(p.y_ + v[1].y_, p.x_ + v[1].x_) == 'S' ;
}
auto pt2(auto const& in)
{
	auto stride = in.extent(0);
	auto height = in.extent(1);
	int cnt { 0 };
	for(auto x { 1}; x < stride - 1; ++x)
		for(auto y { 1 }; y < height - 1; ++y)
		{
			if(in(y, x) == 'A')
			{
				bool se_ = findx(SEx, in, {x, y});
				bool sw_ = findx(SWx, in, {x, y});
				bool nw_ = findx(NWx, in, {x, y});
				bool ne_ = findx(NEx, in, {x, y});
				cnt += (ne_ || sw_) && (se_ || nw_);
			}
		}
	return cnt;
}

int main()
{
	timer t("Overall");
	auto in {get_input()};
	size_t stride { in.first};
    size_t height{ in.second.size() / in.first};
    stdex::mdspan md(in.second.data(), stride, height);
 
	std::cout << "pt1 = " << pt1(md) << "\n";
	std::cout << "pt2 = " << pt2(md) << "\n";
}
