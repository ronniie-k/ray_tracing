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
};