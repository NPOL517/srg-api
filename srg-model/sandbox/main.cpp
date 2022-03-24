#include <iostream>
#include <vector>
#include "RK4.h"
#include "julian.h"

#define _USE_MATH_DEFINES 
#include <math.h>

#include "constants.h"

struct State
{
    double mjd = 0;
    std::array<double, 6> state;
};

int main()
{
    bool ephemOk = DE421.open("lnxp1900p2053.421");
    if (!ephemOk)
    {
        std::cerr << "ephem error" << std::endl;
        return 1;
    }


    std::vector<State> data;
    std::cout << "Reading\n";

    FILE* f = fopen("data.txt", "w");
    if (f == NULL)
    {
        std::cout << "Failed file opening";
        return -1;
    }
    std::cout << "Reading\n";

    int counter = 0;
    std::cout << counter;
    std::array<double, 6> arr = { -3161.946517, 2801.776225, -5322.279908,
                                  -4.795367, -5.966911, -0.292216 };

    int day=1, month=1, year=2000;
    double time_mjd = cal2mjd(day, month, year);
    double h = 100;
    dopri8(time_mjd, arr, h,
           [&](double mjd, const std::array<double, 6>& state)
            {
//                State s;
//                s.mjd = mjd;
//                s.state = state;
                  std::cout << counter << std::endl;
                  counter++;
                  int day, month, year, hour, min, sec, msec;
                  mjd2cal(mjd, day, month, year, hour, min, sec, msec);
                  fprintf(f, "%02d.%02d.%04d_%02d:%02d:%02d.%03d %f %f %f %f %f %f %f\n",
                          day, month, year, hour, min, sec, msec, mjd, state[0],
                          state[1], state[2], state[3], state[4], state[5]);
//                data.push_back(s);
            });
    fclose(f);
    //std::cout << data.size() << std::endl;
    std::cout << "end" << std::endl;
    return 0;
}