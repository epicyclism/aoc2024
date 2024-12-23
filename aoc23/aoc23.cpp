#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <iterator>

#include "ctre_inc.h"
#include "timer.h"

using vertex_id_t = std::pair<char, char>;

auto get_input()
{
	std::map<vertex_id_t, std::set<vertex_id_t>> al;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		auto f = std::make_pair(ln[0], ln[1]);
		auto t = std::make_pair(ln[3], ln[4]);
		al[f].insert(t);
		al[t].insert(f);
	}
	return al;
}

bool is_connected(auto const& al, vertex_id_t f, vertex_id_t t)
{
	for(auto const& v: al.at(f))
		if( v == t)
			return true;
	return false;
}

bool is_clique3(auto const& al, vertex_id_t v1, vertex_id_t v2, vertex_id_t v3)
{
	if(v1 == v2 || v1 == v3 || v2 == v3)
		return false;
	return is_connected(al, v1, v2) && is_connected(al, v1, v3) && is_connected(al, v2, v3);
}

std::ostream& operator<<(std::ostream& o, vertex_id_t v)
{
	o << v.first << v.second;
	return o;
}

auto pt1(auto const& al)
{
	timer t("p1");
	std::vector<std::vector<vertex_id_t>> vg;
	for(auto i1 = al.begin(); i1 != al.end(); ++i1)
	{
		if((*i1).first.first == 't')
		{
			for(auto i2 = al.begin(); i2 != al.end(); ++i2)
			{
				for(auto i3 = al.begin(); i3 != al.end(); ++i3)
				{
					if(is_clique3(al, (*i1).first, (*i2).first, (*i3).first))
					{
						vg.push_back({});
						vg.back().emplace_back((*i1).first);
						vg.back().emplace_back((*i2).first);
						vg.back().emplace_back((*i3).first);
					}
				}
			}
		}
	}
	for(auto& cl: vg)
		std:sort(cl.begin(), cl.end());
	std::sort(vg.begin(), vg.end());
	auto ne = std::unique(vg.begin(), vg.end());

	return std::distance(vg.begin(), ne);
}

std::set<vertex_id_t> form_intersection(std::set<vertex_id_t> const& l, std::set<vertex_id_t> const& r)
{
	std::set<vertex_id_t> rs;
	std::set_intersection(l.begin(), l.end(), r.begin(), r.end(), std::inserter(rs, rs.end()));
	return rs;
}

template<typename C> void bron_kerbosch(auto& g, std::set<vertex_id_t> R, std::set<vertex_id_t> P, std::set<vertex_id_t> X, C cc )
{
	if(P.empty() && X.empty())
	{
		cc(R);
		return;
	}
	auto PC = P;
	for(auto v : P)
	{
		R.insert(v);
		bron_kerbosch(g, R, form_intersection(PC, g[v]), form_intersection(X, g[v]), cc);
		PC.erase(v);
		X.insert(v);
		R.erase(v);
	}
}

auto pt2(auto& al)
{
	timer t("p2");
	std::set<vertex_id_t> P;
	for(auto& m: al)
		P.insert(m.first);
	std::set<vertex_id_t> out;
	bron_kerbosch(al, std::set<vertex_id_t>(), P, std::set<vertex_id_t>(), [&](auto& c) { if (c.size() > out.size()) out.swap(c); });
	std::string s;
	for (auto v : out)
	{
		if (!s.empty())
			s += ',';
		s += v.first;
		s += v.second;
	}
	return s;
}

int main()
{
	auto al = get_input();
	auto p1 = pt1(al);
	std::cout << "pt1 = " << p1 << "\n";
	auto p2 = pt2(al);
	std::cout << "pt2 = " << p2 << "\n";
}
