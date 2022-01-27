#include "RK4.h"
#include <cmath>



// Рунге-Кутта 4 порядка для синуса 
void rk_sin(double& x_0, double& y_0, double h)
{
    //double nu = -cos(x_0);
    double k1 = cos(x_0);
    double k2 = cos(x_0 + h / 2);
    double k3 = cos(x_0 + h / 2);
    double k4 = cos(x_0 + h);

    x_0 = x_0 + h;
    y_0 = y_0 + h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
}


// Функция правых частей
std::array<double, 6> function_of_right_values(std::array<double, 6>& arr)
{
    double mu = 398600.4415;
    double r = sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
    r = r * r * r;
    std::array<double, 6> answ = { arr[3], arr[4], arr[5], -mu * arr[0] / r,
                                  -mu * arr[1] / r, -mu * arr[2] / r };
    return answ;
}


// Рунге-Кутта 4 порядка для задачи двух тел
std::array<double, 6> rk4(std::array<double, 6>& arr_baz, double h)
{
    auto k1_function = function_of_right_values(arr_baz);
    auto arrt = arr_baz;

    for (int i = 0; i < 6; i++)
    {
        arrt[i] = arr_baz[i] + h / 2 * k1_function[i];
    }

    auto k2_function = function_of_right_values(arrt);

    for (int i = 0; i < 6; i++)
    {
        arrt[i] = arr_baz[i] + h / 2 * k2_function[i];
    }

    auto k3_function = function_of_right_values(arrt);

    for (int i = 0; i < 6; i++)
    {
        arrt[i] = arr_baz[i] + h * k3_function[i];
    }

    auto k4_function = function_of_right_values(arrt);

    std::array<double, 6> answer;

    for (int i = 0; i < 6; i++)
    {
        answer[i] = arr_baz[i] + h / 6 * (k1_function[i] + 2 * k2_function[i] + 2 * k3_function[i] + k4_function[i]);
    }

    return answer;
}