#include "Random.h"

Random::Random()
    :m_rng(m_device())
{

}

float Random::getFloatInRangeImpl(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_rng);
}
