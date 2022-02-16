#include "RK4.h"
#include <cmath>



// Таблица Бутчера для метода Рунге-Кутта 4 порядка
std::array<std::array<double, 4>, 3> butchers_a = { {
        //{0, 0, 0, 0}, /////////////////////------------------??????????
        {1.0 / 2, 0, 0, 0},
        {0, 1.0 / 2, 0, 0},
        {0, 0, 1, 0}
        } };
std::array<double, 4> butchers_b = { 1.0 / 6, 1.0 / 3, 1.0 / 3, 1.0 / 6 };
std::array<double, 4> butchers_c = { 0, 1.0 / 2, 1.0 / 2, 1 };

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
std::array<double, 6> function_of_right_values(double time, std::array<double, 6>& arr)
{
    double mu = 398600.4415;
    double r = sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
    r = r * r * r;
    std::array<double, 6> answ = { arr[3], arr[4], arr[5], -mu * arr[0] / r,
                                  -mu * arr[1] / r, -mu * arr[2] / r };
    return answ;
}


// Реализация метода Рунге-Кутта с помощью Таблицы Бутчера
std::array<double, 6> rk4(double& time, std::array<double, 6>& arr_baz, double h)
{
    std::array<std::array<double, 6>, 4> skos;
    skos[0] = function_of_right_values(time + butchers_c[0] * h, arr_baz);
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            double h_koeff = 0;
            for (int k = 0; k < i; k++)
            {
                h_koeff = h_koeff + butchers_a[i - 1][k] * skos[k][j];
            }
            skos[i][j] = arr_baz[j] + h * h_koeff;
        }
        skos[i] = function_of_right_values(time + h * butchers_c[i] / 86400, skos[i]);
    }

    std::array<double, 6> answer;
    for (int j = 0; j < 6; j++)
    {
        double ks = 0;
        for (int i = 0; i < 4; i++)
        {
            ks = ks + butchers_b[i] * skos[i][j];
        }
        answer[j] = arr_baz[j] + h * ks;

    }


    return answer;
}

