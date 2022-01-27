#include <iostream>
#include "RK4.h"

#define _USE_MATH_DEFINES 
#include <math.h> // Äכÿ גûחמגא usage_rk4


// Ïנטלונ גûחמגא rk4
int usage_rk4()
{
    auto path = "C:/Users/VTB/Desktop/file_sin_rk4.txt";
    double x_0 = 0;
    double y_0 = sin(x_0);
    double x_end = 2 * M_PI;
    double h = (2 * M_PI) / 360;

    FILE* f = fopen(path, "w");
    if (f == NULL) {
        std::cout << "Failed file opening";
        return -1;
    }
    else {

        while (x_0 <= x_end)
        {
            // std::cout << x_0 << "\n";
            fprintf(f, "%f %f\n", x_0, y_0);
            rk_sin(x_0, y_0, h);
            // auto file_line = std::to_string(x_0) + " " + std::to_string(y_0) + "\n";

        }
    }
    fclose(f);

    std::array<double, 6> arr = { -3161.946517, 2801.776225, -5322.279908, -4.795367, -5.966911, -0.292216 };
    std::array<double, 6> arr_modified = rk4(arr, 1);
    FILE* n = fopen("C:/Users/VTB/Desktop/rk4.txt", "w");
    if (n == NULL) {
        std::cout << "Failed file opening";
        return -1;
    }
    else {
        for (int h = 1; h < 60 * 60 * 1.5; h++)
        {
            fprintf(f, "%f %f %f\n", arr_modified[0], arr_modified[1], arr_modified[2]);
            arr_modified = rk4(arr_modified, 1);
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
    //std::cout << "SANDBOX" << std::endl;
    return 0;
}
