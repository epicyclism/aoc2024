#include "ctre.hpp"

template<typename T> T sv_to_t ( std::string_view sv)
{
	T t { 0 };
	std::from_chars(sv.data(), sv.data() + sv.size(), t);
	return t;
}

template<typename T> T sv_to_t(std::string_view sv, int b)
{
	T t{ 0 };
	std::from_chars(sv.data(), sv.data() + sv.size(), t, b);
	return t;
}