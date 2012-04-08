#ifndef TIMER_H
#define TIMER_H

#include "TimerMacros.h"

#ifdef _WIN32

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
        int res = clock_getres(CLOCK_MONOTONIC, &resolution);
        int stt = clock_gettime(CLOCK_MONOTONIC, &start_time);
        if ((res == -1) || (stt == -1))
        {
            printf("ERROR : res %d : stt %d\n", res, stt);
        }
        else 
        { 
            printf ("LOGGING : res and stt OK\n"); 
        }
    }

    double Seconds()
    {
        timespec current_time;
        int result = clock_gettime(CLOCK_MONOTONIC, &current_time);
        int seconds = 0;
        ((result == 0) ? seconds = current_time.tv_sec-start_time.tv_sec : 
            seconds = result);
        return seconds;
    }

    double Milliseconds() 
    {
        return Seconds() * 1000.0;
    }

private:
    timespec start_time;
    timespec resolution;
};

#endif

//namespace timing
//{
        static bool ShouldUpdate(boost::shared_ptr<Timer> timer, double last_update, int frequency)
        {
            return timer->Milliseconds() > last_update / frequency;
        }

        static unsigned long ElapsedTime(unsigned int start_time)
        {
	    auto difference = GET_TIME - start_time;
	    return difference;
        }
//}

#endif
