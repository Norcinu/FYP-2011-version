#ifndef TIMER_H
#define TIMER_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include <MMSystem.h>
#include <boost/shared_ptr.hpp>

class Timer
{
public:
	Timer(void) { Reset(); }

	~Timer(void) {}

	void Reset()
	{
		unsigned long long freq;
		QueryPerformanceFrequency((LARGE_INTEGER *) &freq);
		m_frequency = 1.0 / (double)freq;
		QueryPerformanceCounter((LARGE_INTEGER *) &m_baseTime);
	}

	double Seconds()
	{
		unsigned long long value;
		QueryPerformanceCounter((LARGE_INTEGER *) &value);
		return (value - m_baseTime) * m_frequency;
	}

	double Milliseconds() { return Seconds() * 1000.0; }

private:
	double m_frequency;
	unsigned long long m_baseTime;
};
#else
// Linux version
class Timer 
{
public:
    Timer() { Reset(); }
    ~Timer() {}

    void Reset() 
    {
    }

    double Seconds()
    {
        return 1.00;
    }

    double Milliseconds() 
    {
        return 1.00;
    }
private:
    
};

#endif

namespace timing
{
	static bool ShouldUpdate(boost::shared_ptr<Timer> timer, double last_update, int frequency)
	{
		return timer->Milliseconds() > last_update / frequency;
	}

	static unsigned long ElapsedTime(unsigned int start_time)
	{
		auto difference = timeGetTime() - start_time;
		return difference;
	}
}

#endif
