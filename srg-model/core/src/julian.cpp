#include "julian.h"
#include <cmath>

// Календарная дата -> Юлианская дата.
double cal2jd(int day, int month, int year, int hour, int min,
    int sec, int msec)
{
    return 0;
}

// Юлианская дата -> Календарная дата.
void jd2cal(double jd, int& day, int& month, int& year, int& hour,
    int& min, int& sec, int& msec)
{
    day = month = year = hour = min = sec = msec = 0;
}

// Юлианская дата -> Модифицированная юлианская дата.
double jd2mjd(double jd)
{
    return 0;
}

// Модифицированная юлианская дата -> Юлианская дата.
double mjd2jd(double mjd)
{
    return 0;
}

// Календарная дата -> Модифицированная юлианская дата.
double cal2mjd(int day, int month, int year, int hour, int min, int sec,
    int msec)
{
    return 0;
}

// Модифицированная юлианская дата -> Календарная дата.
void mjd2cal(double mjd, int& day, int& month, int& year, int& hour, int& min,
    int& sec, int& msec)
{
    day = month = year = hour = min = sec = msec = 0;
}