#include "Timer.hpp"

Timer::Timer() noexcept
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

float Timer::ElapsedTime() const noexcept
{
	using namespace std::chrono;
	if (isRunning)
	{
		auto elapsed = std::chrono::duration<float, std::centi>(high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<float, std::centi>(stop - start);
		return elapsed.count();
	}
}

bool Timer::IsRunning() const noexcept
{
	return isRunning;
}

bool Timer::Stop() noexcept
{
	if (!isRunning)
		return false;
	else
	{
		isRunning = false;
		stop = std::chrono::high_resolution_clock::now();
		return true;
	}
}

bool Timer::Start() noexcept
{
	if (isRunning)
		return false;
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isRunning = true;
		return true;
	}
}

void Timer::Restart() noexcept
{
	isRunning = true;
	start = std::chrono::high_resolution_clock::now();
}