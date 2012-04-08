#ifndef TIMER_HELPER_MACROS_H
#define TIMER_HELPER_MACROS_H

/**
    Helper macros for making the Timer class platform 
    cross platform.
**/

#include <boost/shared_ptr.hpp>

#ifdef _WIN32

    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #include <windows.h>
    #include <MMSystem.h> // is this needed?
    #define SLEEP(frequency) Sleep(frequency)
    #define GET_TIME timeGetTime()
    #define TIME_VALUE DWORD

#else
    
    #include <time.h>
    #define GET_TIME seg_get_time() 
    #define SLEEP(frequency) seg_sleep(frequency)
    #define TIME_VALUE timespec
    
    void seg_sleep(const int ms) 
    {
        timespec time;
        time.tv_sec = ms / 1000;
        time.tv_nsec = (ms % 1000) * 1000;
        
        nanosleep(&time, NULL);
    }

    int seg_get_time()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC, &time);
        return time.tv_sec * 1000;
    }

#endif // _WIN32

#endif // TIMER_HELPER_MACROS_H


