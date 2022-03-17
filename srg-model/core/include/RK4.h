#ifndef RK4_H
#define RK4_H

#include <array>
#include <functional>


// Тип вектора состояния
enum state_type
{
    STRAIGHT,
    ESTIMATED
};

// Возврат модуля вектора
double vec_mod(const double a[3]);

// Рунге-Кутта 4 порядка для синуса 
void rk_sin(double& x_0, double& y_0, double h);

// Функция правых частей
std::array<double, 6> function_of_right_values(double time,
                                               const std::array<double, 6>& rv);

// Создание матрицы скосов
std::array<std::array<double, 6>, 13> make_slopes(double mjd,
                                                  const std::array<double, 6>& init_state,
                                                  double h_sec);

// Определение вектора состояния (фактического или оценочного)
std::array<double, 6> make_state (const std::array<std::array<double, 6>, 13>& slopes,
                                  const std::array<double, 6>& init_state,
                                  double h_sec, state_type type = STRAIGHT);

// Реализация метода Рунге-Кутта с помощью Таблицы Бутчера с заданным шагом
void dopri8_fixed(double& mjd, std::array<double, 6>& state, double h_sec);

// Реализация метода Рунге-Кутта с динамическим определением шага по времени
void dopri8_auto(double mjd, const std::array<double, 6>& state, double& h_sec,
    std::array<double, 6>& stateNext);

using CallBack = std::function<void(double mjd,
    const std::array<double,6>& state)>;

// Обертка для метода Рунге-Кутта с заданным интервальным значением времени
void dopri8(double &time, std::array<double, 6>& state, double interval,
    CallBack callBack = {});


#endif // RK4_H