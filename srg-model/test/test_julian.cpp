#include <gtest/gtest.h>
#include "julian.h"

TEST(julian, cal2jd)
{
    // 01.01.2000 00:00:00.000 | 2451545.5
    {
        const int day   = 1;
        const int month = 1;
        const int year  = 2000;
        const int hour  = 0;
        const int min   = 0;
        const int sec   = 0;
        const int msec  = 0;

        const int cal[7] = { day, month, year, hour, min, sec, msec };

        double jd = 2451544.5;

        EXPECT_EQ(cal2jd(day, month, year, hour, min, sec, msec), jd);
    }

    // 01.01.2000 12:13:14.567 | 2451545.0091963774
    {
        const int day   = 1;
        const int month = 1;
        const int year  = 2000;
        const int hour  = 12;
        const int min   = 13;
        const int sec   = 14;
        const int msec  = 567;

        const int cal[7] = { day, month, year, hour, min, sec, msec };

        double jd = 2451545.0091963774;

        EXPECT_DOUBLE_EQ(cal2jd(day, month, year, hour, min, sec, msec), jd);
    }
}

TEST(julian, jd2cal)
{
    // 01.01.2000 00:00:00.000 | 2451545.5
    {
        const double jd = 2451544.5;

        int day, month, year, hour, min, sec, msec;
        jd2cal(jd, day, month, year, hour, min, sec, msec);

        EXPECT_EQ(day,   1);
        EXPECT_EQ(month, 1);
        EXPECT_EQ(year,  2000);
        EXPECT_EQ(hour,  0);
        EXPECT_EQ(min,   0);
        EXPECT_EQ(sec,   0);
        EXPECT_EQ(msec,  0);
    }

    // 01.01.2000 12:13:14.567 | 2451545.0091963774
    {
        const double jd = 2451545.0091963774;

        int day, month, year, hour, min, sec, msec;
        jd2cal(jd, day, month, year, hour, min, sec, msec);

        EXPECT_EQ(day,   1);
        EXPECT_EQ(month, 1);
        EXPECT_EQ(year,  2000);
        EXPECT_EQ(hour,  12);
        EXPECT_EQ(min,   13);
        EXPECT_EQ(sec,   14);
        EXPECT_EQ(msec,  567);
    }
}

TEST(julian, jd2mjd)
{
    EXPECT_EQ(jd2mjd(0), -2400000.5);
}

TEST(julian, mjd2jd)
{
    EXPECT_EQ(mjd2jd(0), 2400000.5);
}

TEST(julian, cal2mjd)
{
    // 01.01.2000 00:00:00.000 | 51545
    {
        const int day   = 1;
        const int month = 1;
        const int year  = 2000;
        const int hour  = 0;
        const int min   = 0;
        const int sec   = 0;
        const int msec  = 0;

        const int cal[7] = { day, month, year, hour, min, sec, msec };

        double mjd = 51544;

        EXPECT_EQ(cal2mjd(day, month, year, hour, min, sec, msec), mjd);
    }

    // 01.01.2000 12:13:14.567 | 51544.509196377359
    {
        const int day   = 1;
        const int month = 1;
        const int year  = 2000;
        const int hour  = 12;
        const int min   = 13;
        const int sec   = 14;
        const int msec  = 567;

        const int cal[7] = { day, month, year, hour, min, sec, msec };

        double mjd = 51544.509196377359;

        EXPECT_DOUBLE_EQ(cal2mjd(day, month, year, hour, min, sec, msec), mjd);
    }  
}

TEST(julian, mjd2cal)
{
    // 01.01.2000 00:00:00.000 | 51545
    {
        const double mjd = 51544;

        int day, month, year, hour, min, sec, msec;
        mjd2cal(mjd, day, month, year, hour, min, sec, msec);

        EXPECT_EQ(day,   1);
        EXPECT_EQ(month, 1);
        EXPECT_EQ(year,  2000);
        EXPECT_EQ(hour,  0);
        EXPECT_EQ(min,   0);
        EXPECT_EQ(sec,   0);
        EXPECT_EQ(msec,  0);
    }

    // 01.01.2000 12:13:14.567 | 51544.509196377359
    {
        const double jd = 51544.509196377359;

        int day, month, year, hour, min, sec, msec;
        mjd2cal(jd, day, month, year, hour, min, sec, msec);

        EXPECT_EQ(day,   1);
        EXPECT_EQ(month, 1);
        EXPECT_EQ(year,  2000);
        EXPECT_EQ(hour,  12);
        EXPECT_EQ(min,   13);
        EXPECT_EQ(sec,   14);
        EXPECT_EQ(msec,  567);
    }
}
