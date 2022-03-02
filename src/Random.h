#pragma once

#include<random>

class Random
{
public:
	//singleton declarations
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;
	Random(Random&&) = delete;
	Random& operator=(Random&&) = delete;
public:
	~Random() = default;
	static Random& get() { static Random r; return r; }

	static float getFloatInRange(float min, float max) { return get().getFloatInRangeImpl(min, max); }
private:
	Random();
	float getFloatInRangeImpl(float min, float max);

	std::random_device m_device;
	std::mt19937 m_rng;

	//this is slower
	//values taken from https://en.cppreference.com/w/cpp/numeric/random/linear_congruential_engine
	//std::linear_congruential_engine<std::uint_fast32_t, 16807, 0, 2147483647> m_rng;
	//uint32_t m_seed;
};