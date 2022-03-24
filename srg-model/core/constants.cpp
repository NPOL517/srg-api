#include "constants.h"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846;
#endif

const double R2D = 180.0 / M_PI;
const double D2R = M_PI / 180.0;

const double GM_EARTH = 398600.4415;

dph::EphemerisRelease DE421;
