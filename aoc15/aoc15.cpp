#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <numeric>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

auto get_input()
{
	std::vector<char> w;
	int s{0};
	std::string m;

	std::string ln;
	bool wm{false};
	while(std::getline(std::cin, ln))
	{
		if(s == 0)
			s = ln.size();
		if(ln.empty())
			wm = true;
		else
		{
			if(wm)
				m += ln;
			else
				w.insert(w.end(), ln.begin(), ln.end());
		}
	}

	return std::make_tuple(w, s, m);
}

struct pt
{
	int x_;
	int y_;
	constexpr pt() : x_ { 0}, y_{0}
	{}
	constexpr pt(int x, int y) : x_{x}, y_{y}
	{}
};

pt from_dir(char c)
{
	switch(c)
	{
		case '<':
			return {-1, 0};
		case '>':
			return {1, 0};
		case '^':
			return {0, -1};
		case 'v':
			return {0, 1};
		default: // tilt
			return {0, 0};
	}
}

pt move1(pt p, pt d, auto& md)
{
	pt p2{p.x_ + d.x_, p.y_ + d.y_};
	while(md(p2.y_, p2.x_) != '#')
	{
		if(md(p2.y_, p2.x_) == '.')
		{
			std::swap(md(p.y_ + d.y_, p.x_ + d.x_), md(p2.y_, p2.x_));
			std::swap(md(p.y_ + d.y_, p.x_ + d.x_), md(p.y_, p.x_));
			return {p.x_ + d.x_, p.y_ + d.y_};
		}
		p2.x_ += d.x_;
		p2.y_ += d.y_;
	}
	return p;
}

int64_t compute_gps1(auto& md)
{
	int64_t t{0};
	for(int64_t y{0}; y < md.extent(0); ++y)
		for(int64_t x{0}; x < md.extent(1) ; ++x)
			if(md(y, x) == 'O')
				t += (x + 100 * y);
	return t;
}

void print(auto& md)
{
	for(int64_t y{0}; y < md.extent(0); ++y)
	{
		for(int64_t x{0}; x < md.extent(1) ; ++x)
		{
			std::cout << md(y, x);
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

int64_t pt1(std::vector<char> w, int s, std::string const& m)
{
	stdex::mdspan md(w.data(), s, s);
	pt p;
	for(int x = 0 ; x < s; ++x)
		for(int y = 0 ; y < s; ++y)
			if(md(y, x) == '@')
			{
				p.x_ = x;
				p.y_ = y;
				goto got;
			}
got:
	for(auto mv : m)
		p = move1(p, from_dir(mv), md);
	
	return compute_gps1(md);
}


std::vector<char> expand(std::vector<char> const& v)
{
	std::vector<char> vr;
	vr.reserve(v.size() * 2);
	for(auto c: v)
	{
		switch(c)
		{
			case '.':
				vr.push_back('.');
				vr.push_back('.');
				break;
			case '#':
				vr.push_back('#');
				vr.push_back('#');
				break;
			case 'O':
				vr.push_back('[');
				vr.push_back(']');
				break;
			case '@':
				vr.push_back('@');
				vr.push_back('.');
				break;
			default:
				std::cout << "erk\n";
				break;
		}
	}
	return vr;
}

int64_t compute_gps2(auto& md)
{
	int64_t t{0};
	for(uint64_t y{0}; y < md.extent(0); ++y)
		for(uint64_t x{0}; x < md.extent(1) ; ++x)
			if(md(y, x) == '[')
				t += (x + 100 * y);
	return t;
}

bool can_move( pt d, auto& md, auto& to_move)
{
	for(auto p : to_move)
		if(md(p.y_ + d.y_, p.x_ + d.x_) == '#')
			return false;

	return true;
}

void find_set(pt p, pt d, auto& md, auto& to_move)
{
	std::vector<bool> fnd(100*50);
	std::queue<pt> q;
	q.push({p.x_, p.y_ });
	fnd[p.y_ * 100 + p.x_] = true;
	while(!q.empty())
	{
		auto pw = q.front();
		q.pop();
		if(md(pw.y_, pw.x_) != '.' && md(pw.y_, pw.x_) != '#')
		{
			to_move.push_back(pw);
			if(md(pw.y_ , pw.x_ - 1) == '[')
			{
				if(!fnd[pw.y_ * 100 + pw.x_ - 1])
				{
					fnd[pw.y_ * 100 + pw.x_ - 1] = true;
					q.push({pw.x_ - 1, pw.y_});
				}
			}
			if(md(pw.y_, pw.x_ + 1) == ']')
			{
				if(!fnd[pw.y_ * 100 + pw.x_ + 1])
				{
					fnd[pw.y_ * 100 + pw.x_ + 1] = true;
					q.push({pw.x_ + 1, pw.y_});
				}
			}
			if(md(pw.y_ + d.y_, pw.x_) == '[' || md(pw.y_ + d.y_, pw.x_) == ']')
			{
				if(!fnd[(pw.y_ + d.y_) * 100 + pw.x_])
				{
					fnd[(pw.y_ + d.y_) * 100 + pw.x_] = true;
					q.push({pw.x_, pw.y_ + d.y_});
				}
			}
		}
	}
}

pt move2(pt p, pt d, auto& md)
{
	std::vector<pt> to_move;

	if( d.y_ == 0) // horizontal
	{
		to_move.push_back(p);
		pt p2{p.x_ + d.x_, p.y_ + d.y_};
		while(md(p2.y_, p2.x_) != '#')
		{
			if(md(p2.y_, p2.x_) == '.')
				break;
			to_move.push_back(p2);
			p2.x_ += d.x_;
			p2.y_ += d.y_;
		}
		if(md(p2.y_, p2.x_) == '#')
			to_move.clear();
	}
	else
	{
		find_set(p, d, md, to_move);
		if(!can_move(d, md, to_move))
			to_move.clear();
	}
	if(!to_move.empty())
	{
		std::sort(to_move.begin(), to_move.end(), [&](auto& l, auto& r)
		{
			if(d.y_ < 0)
				return l.y_ < r.y_;
			if(d.y_ > 0)
				return l.y_ > r.y_;
			if(d.x_ < 0)
				return l.x_ < r.x_;
			if(d.x_ > 0)
				return l.x_ > r.x_;
			// not reached
			return false;
		});
		for(auto& p: to_move)
		{
			std::swap(md(p.y_, p.x_), md(p.y_ + d.y_, p.x_ + d.x_));
			md(p.y_, p.x_) = '.';
		}
		return {p.x_ + d.x_, p.y_ + d.y_};
	}
	return p;
}

int64_t pt2(std::vector<char> const& w, int s, std::string const& m)
{
	auto ww{expand(w)};
	stdex::mdspan md(ww.data(), s, s * 2);
	pt p;
	for(int64_t y{0}; y < md.extent(0); ++y)
		for(int64_t x{0}; x < md.extent(1); ++x)
			if(md(y, x) == '@')
			{
				p.x_ = x;
				p.y_ = y;
				goto got;
			}
got:
	for(auto mv : m)
		p = move2(p, from_dir(mv), md);
	
	return compute_gps2(md);
}

int main()
{
	auto [w, s, m] = get_input();
	std::cout << "pt1 = " << pt1(w, s, m) << "\n";
	std::cout << "pt2 = " << pt2(w, s, m) << "\n";
}
