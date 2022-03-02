#include "Random.h"

#include<chrono>

Random::Random()
    //:m_seed(std::chrono::system_clock::now().time_since_epoch().count())
    :m_rng(m_device())
{
}

float Random::getFloatInRangeImpl(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_rng);
}
