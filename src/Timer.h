#pragma once

#include<string>
#include<chrono>
#include<unordered_map>

//not designed specifically for this project in mind, not a general timer
using time_point_t = std::chrono::steady_clock::time_point;

class Timer
{
public:
	//singleton declarations
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	~Timer() = default;
	static Timer& get() { static Timer r; return r; }

	static void add(const std::string& name, const long long& time) { get().addImpl(name, time); }
	static void print() { get().printImpl(); }
private:
	Timer();

	void addImpl(const std::string& name, const long long& time);
	void printImpl();
private:
	std::unordered_map<std::string, std::pair<double, int>> m_averages;
};