#ifndef JULIAN_H
#define JULIAN_H

// Календарная дата -> Юлианская дата.
double cal2jd(int day, int month, int year, int hour = 0, int min = 0,
    int sec = 0, int msec = 0);

// Юлианская дата -> Календарная дата.
void jd2cal(double jd, int& day, int& month, int& year, int& hour, int& min,
    int& sec, int& msec);

// Юлианская дата -> Модифицированная юлианская дата.
double jd2mjd(double jd);

// Модифицированная юлианская дата -> Юлианская дата.
double mjd2jd(double mjd);

// Календарная дата -> Модифицированная юлианская дата.
double cal2mjd(int day, int month, int year, int hour = 0, int min = 0,
    int sec = 0, int msec = 0);

// Модифицированная юлианская дата -> Календарная дата.
void mjd2cal(double mjd, int& day, int& month, int& year, int& hour, int& min,
    int& sec, int& msec);

#endif // JULIAN_H
