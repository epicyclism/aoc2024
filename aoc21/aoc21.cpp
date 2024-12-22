#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <span>

#include "graph.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::vector<char>> vv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		std::vector<char> v;
		for(auto c: ln)
		{
			if(c == 'A')
				v.push_back(0xa);
			else
				v.push_back(c - '0');
		}
		vv.emplace_back(v);
	}
	return vv;
}

// all shortest paths
auto bfs(auto const& g, char id_from, char id_to) 
{
	std::vector<std::vector<char>> routes;
   struct q_t
   {
      char v_;
      std::vector<char> rt_;
   };
   std::queue<q_t> q;
   q.emplace(id_from);
   while (!q.empty())
   {
      auto u = q.front(); q.pop();
      if(u.v_ == id_to)
      {
         if(routes.empty() || u.rt_.size() == routes.back().size())   
            routes.push_back(u.rt_);
         else
            break;
      }
      else
      {
		   char d = 0;
         for(auto e : g[u.v_])
         {
            if (valid_vertex_id(e))
            {
               q.emplace(e, u.rt_);
               q.back().rt_.push_back(d);
            }
            ++d;
         }
      }
   }
   return routes;
}

// 789
// 456
// 123
//  0A
//
std::vector<std::array<char, 4>> keypad
{
	{-1, 2, 0xa, -1}, // 0
	{-1, 4, 2, -1}, // 1
	{1, 5, 3, 0}, // 2
	{2, 6, -1, 0xa}, // 3
	{-1, 7, 5, 1}, // 4
	{4, 8, 6, 2}, // 5
	{5, 9, -1, 3}, // 6
	{-1, -1, 8, 4}, // 7
	{7, -1, 9, 5}, // 8
	{8, -1, -1, 6}, // 9
	{0, 3, -1, -1}  // A
};

//  ^A    1 a
// <v>  0 3 2
//
std::vector<std::array<char, 4>> directionpad
{
	{-1, -1, 3, -1},  // 0
	{-1, -1, 0xa, 3}, // 1
	{3, 0xa, -1, -1}, // 2
	{0, 1, 2, -1}, // 3
	{-1, -1, -1, -1}, // 4
	{-1, -1, -1, -1}, // 5	
	{-1, -1, -1, -1}, // 6
	{-1, -1, -1, -1}, // 7
	{-1, -1, -1, -1}, // 8
	{-1, -1, -1, -1}, // 9
	{1, -1, -1, 2},   // a
};

template<typename G> void build_sequences(G const& g, auto const& keys, size_t k, char from, std::vector<char> pth, auto& out)
{
   if( k == keys.size())
      out.emplace_back(pth);
   else
   {
      auto ps = bfs(g, from, keys[k]);
      for(auto& p: ps)
      {
         std::vector<char> pp(pth);
         pp.insert(pp.end(), p.begin(), p.end());
         pp.push_back(0xa);
         build_sequences(g, keys, k + 1, keys[k], std::move(pp), out);
      }
   }
}

using cache_t = std::map<std::pair<int, std::vector<char>>, int64_t>;

int64_t shortest_sequence(std::vector<char> k, int depth, cache_t& cache)
{
   if( depth == 0)
      return k.size();
   auto cit = cache.find({depth, k});
   if(cit != cache.end())
      return (*cit).second;
   auto it{k.begin()};
	auto it2{it};
   int64_t total{0};
	while(it2 != k.end())
	{
		while(*it2 != 0xa)
			++it2;
		++it2;
		auto sp = std::span(it, it2);
      std::vector<std::vector<char>> vp;
      build_sequences(directionpad, sp, 0, 0xa, {}, vp);
      int64_t mn{0x7fffffffffffffff};
      for(auto& sq: vp)
      {
         auto tm = shortest_sequence(sq, depth - 1, cache);
         if(tm < mn)
            mn = tm;
      }
		it = it2;
      total += mn;
	}
   cache[{depth, k}] = total;
   return total;
}

int64_t to_i(auto& v)
{
	int64_t t{0};
	for(auto c: v)
	{
		if(c>9)
			break;
		t *= 10;
		t += c;
	}
	return t;
}

int64_t pt12(auto const& vv, int depth)
{
	timer t("pt12");
	int64_t r{0};
   cache_t cache;
	for(auto& v: vv)
	{
		int id = to_i(v);
      std::vector<std::vector<char>> vp;
      build_sequences(keypad, v, 0, 0xa, {}, vp);
      int64_t mn{0x7fffffffffffffff};
      for(auto p0: vp)
      {
         auto t = shortest_sequence(p0, depth, cache );
         if( t < mn)
            mn = t;
      }
      r += mn * id;
	}
	return r;
}

int main()
{
	auto vv = get_input();
	auto p1{pt12(vv, 2)};
	std::cout << "pt1 = " << p1 << "\n";

	auto p2{pt12(vv, 25)};
	std::cout << "pt2 = " << p2 << "\n";
}