#include <iostream>
#include <vector>
#include "RK4.h"

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


    return 0;

    std::vector<State> data;




    std::array<double, 6> arr = { -3161.946517, 2801.776225, -5322.279908, -4.795367, -5.966911, -0.292216 };
    double time = 0;
    double h = 100;
    dopri8(time, arr, h,
           [&](double mjd, const std::array<double, 6>& state)
            {
                State s;
                s.mjd = mjd;
                s.state = state;
                data.push_back(s);
            });

    std::cout << data.size() << std::endl;
    std::cout << "end" << std::endl;
    return 0;
}