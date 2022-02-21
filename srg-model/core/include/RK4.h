#ifndef RK4_H
#define RK4_H
#include <array>

// Рунге-Кутта 4 порядка для синуса 
void rk_sin(double& x_0, double& y_0, double h);

// Функция правых частей
std::array<double, 6> function_of_right_values(std::array<double, 6>& arr);

// Реализация метода Рунге-Кутта с помощью Таблицы Бутчера
void dopri8_fixed(double& time, std::array<double, 6>& arr_baz, double h);

// Обертка для метода Рунге-Кутта с заданным интервальным значением
void dopri8(double &time, std::array<double, 6>& state, double interval);


#endif // RK4_H