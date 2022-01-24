#include "julian.h"
#include <cmath>

// Календарная дата -> Юлианская дата.
double cal2jd(int day, int month, int year, int hour, int min,
    int sec, int msec)
{
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 
        - 32045; // Номер юлианского дня (без дробной части)
    double jd = double(jdn) + (double(hour) - 12) / 24 + double(min) / 1440 + 
        double(sec) / 86400 + double(msec) / 8.64e7; // Юлианская дата (с дробной частью)
    return jd;
}

// Юлианская дата -> Календарная дата.
void jd2cal(double jd, int& day, int& month, int& year, int& hour,
    int& min, int& sec, int& msec)
{
    int jdn = jd + 0.5;
    int a = jdn + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - (146097 * b) / 4;
    int d = (4 * c + 3) / 1461;
    int e = c - (1461 * d) / 4;
    int m = (5 * e + 2) / 153;

    day = e - (153 * m + 2) / 5 + 1;
    month = m + 3 - 12 * (m / 10);
    year = 100 * b + d - 4800 + m / 10;

    double fraction = jd - jdn;
    int msecs_total = 8.64e7 * fraction + 4.32e7; // Число миллисекунд, прошедшее с начала суток указанного дня
    int pr = (msecs_total % 3600000);

    hour = (msecs_total / 3600000); // Часы
    min = pr / 60000; // Минуты
    sec = (pr % 60000) / 1000; //Секунды
    msec = (pr % 60000) % 1000;
}

// Юлианская дата -> Модифицированная юлианская дата.
double jd2mjd(double jd)
{
    return (jd - 2400000.5);
}

// Модифицированная юлианская дата -> Юлианская дата.
double mjd2jd(double mjd)
{
    return (mjd + 2400000.5);
}

// Календарная дата -> Модифицированная юлианская дата.
double cal2mjd(int day, int month, int year, int hour, int min, int sec,
    int msec)
{
    return (jd2mjd(cal2jd(day, month, year, hour, min, sec, msec)));
}

// Модифицированная юлианская дата -> Календарная дата.
void mjd2cal(double mjd, int& day, int& month, int& year, int& hour, int& min,
    int& sec, int& msec)
{
    double jd = mjd2jd(mjd);

    jd2cal(jd, day, month, year, hour, min, sec, msec);
}