#include <iostream>
#include "RK4.h"

#define _USE_MATH_DEFINES 
#include <math.h>


// Пример вызова dopri8
int usage_dopri8(double &time, std::array<double, 6> arr, double h)
{
    dopri8(time, arr, h);
    return 0;
}

int main()
{
    std::array<double, 6> arr = { -3161.946517, 2801.776225, -5322.279908, -4.795367, -5.966911, -0.292216 };
    std::array<double, 6> arr2;
    double time = 0;
    double h = 1;
    //dopri8_auto(time, arr, h, arr2);
    std::cout << h << "\n" << arr2[0] << " " << arr2[1] << " " << arr2[2];
    //usage_dopri8(time, arr, 86400);
    
    //for (int i = 0; i < 6; i++)
    //    std::cout << arr[i] << " ";
    //std::cout << "Time: " << time;
    return 0;
}