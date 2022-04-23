#include "Timer.h"

#include"Log.h"

Timer::Timer()
{
	m_averages["inShadow()"] = std::pair<double, int>(0, 0);
	m_averages["tracePath()"] = std::pair<double, int>(0, 0);
	m_averages["setPixelColor()"] = std::pair<double, int>(0, 0);
	m_averages["getRayThroughPixel()"] = std::pair<double, int>(0, 0);
	m_averages["triIntersection()"] = std::pair<double, int>(0, 0);
	m_averages["sphereIntersection()"] = std::pair<double, int>(0, 0);
	m_averages["triIntersection()"] = std::pair<double, int>(0, 0);
	m_averages["blinnPhong()"] = std::pair<double, int>(0, 0);
}

void Timer::addImpl(const std::string& name, const long long& time)
{
	double old = m_averages[name].first;
	int& count = m_averages[name].second;
	m_averages[name].first = old + ((time - old) / ++count);
}

void Timer::printImpl()
{
	Log::info("average time taken in inShadow():           {} ns, called {} times", m_averages["inShadow()"].first, m_averages["inShadow()"].second);
	Log::info("average time taken in tracePath():          {} ns, called {} times", m_averages["tracePath()"].first, m_averages["tracePath()"].second);
	Log::info("average time taken in setPixelColor():      {} ns, called {} times", m_averages["setPixelColor()"].first, m_averages["setPixelColor()"].second);
	Log::info("average time taken in getRayThroughPixel(): {} ns, called {} times", m_averages["getRayThroughPixel()"].first, m_averages["getRayThroughPixel()"].second);
	Log::info("average time taken in triIntersection():    {} ns, called {} times", m_averages["triIntersection()"].first, m_averages["triIntersection()"].second);
	Log::info("average time taken in sphereIntersection(): {} ns, called {} times", m_averages["sphereIntersection()"].first, m_averages["sphereIntersection()"].second);
	Log::info("average time taken in blinnPhong():         {} ns, called {} times", m_averages["blinnPhong()"].first, m_averages["blinnPhong()"].second);
}
