#ifndef RK4_H
#define RK4_H
#include <array>

// Рунге-Кутта 4 порядка для синуса 
void rk_sin(double& x_0, double& y_0, double h);

// Функция правых частей
std::array<double, 6> function_of_right_values(std::array<double, 6>& arr);

// Рунге-Кутта 4 порядка для задачи двух тел
std::array<double, 6> rk4(double& time, std::array<double, 6>& arr_baz, double h);

#endif // RK4_H