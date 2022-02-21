#include <iostream>
#include "RK4.h"

#define _USE_MATH_DEFINES 
#include <math.h>


// Пример вызова rk4
int usage_rk4()
{
    std::array<double, 6> arr = { -3161.946517, 2801.776225, -5322.279908, -4.795367, -5.966911, -0.292216 };
    double time = 0;
    std::array<double, 6> arr_modified = rk4(time, arr, 1);
    FILE* n = fopen("rk4.txt", "w");
    if (n == NULL) {
        std::cout << "Failed file opening";
        return -1;
    }
    else {
        for (int h = 1; h < 5579; h++)
        {
            fprintf(n, "%f %f %f\n", arr_modified[0], arr_modified[1], arr_modified[2]);
            arr_modified = rk4(time, arr_modified, 1);
        }
    }
    for (auto v : arr)
        std::cout << v << std::endl;
    std::cout << "\n\n\n";
    for (auto v : arr_modified)
        std::cout << v << std::endl;
    return 0;
}



int main()
{
    usage_rk4();
    return 0;
}
