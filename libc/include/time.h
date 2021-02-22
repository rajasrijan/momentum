#ifndef TIME_T
#define TIME_T
#include <stdint.h>

typedef uint64_t time_t;

struct timeval
{
    time_t tv_sec;
    time_t tv_usec;
};

struct timezone
{
    int tz_minuteswest;
    int tz_dsttime;
};

struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

#endif