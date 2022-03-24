#include "RK4.h"
#include <cmath>
#include <algorithm>
#include "constants.h"
#include "julian.h"


// Возврат модуля вектора
double vec_mod(const double a[3])
{
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}



// Таблица Бутчера для метода Рунге-Кутта
std::array<std::array<double, 12>, 12> butchers_a = { {
        {1.0 / 18},
        {1.0/48, 1.0 / 16},
        {1.0/32, 0, 3.0/32},
        {5.0/16, 0, -75.0/64, 75.0/64},
        {3.0/80, 0, 0, 3.0/16, 3.0/20},
        {29443841.0/614563906, 0, 0, 77736538.0/692538347,
         -28693883.0/1125000000, 23124283.0/1800000000},
        {16016141.0/946692911, 0, 0, 61564180.0/158732637,
         22789713.0/633445777, 545815736.0/2771057229, -180193667.0/1043307555},
        {39632708.0/573591083, 0, 0, -433636366.0/683701615,
         -421739975.0/2616292301, 100302831.0/723423059, 790204164.0/839813087,
         800635310.0/3783071287},
        {246121993.0/1340847787, 0, 0, -37695042795.0/15268766246,
         -309121744.0/1061227803, -12992083.0/490766935, 
         6005943493.0/2108947869, 393006217.0/1396673457, 
         123872331.0/1001029789},
        {-1028468189.0/846180014, 0, 0, 8478235783.0/508512852,
         1311729495.0/1432422823, -10304129995.0/1701304382, 
         -48777925059.0/3047939560, 15336726248.0/1032824649, 
         -45442868181.0/3398467696, 3065993473.0/597172653},
        {185892177.0/718116043, 0, 0, -3185094517.0/667107341,
         -477755414.0/1098053517, -703635378.0/230739211, 5731566787.0/1027545527,
         5232866602.0/850066563, -4093664535.0/808688257, 3962137247.0/1805957418,
         65686358.0/487910083},
        {403863854.0/491063109, 0, 0, -5068492393.0/434740067, 
         -411421997.0/543043805, 652783627.0/914296604, 11173962825/925320556,
         -13158990841.0/6184727034,
         3936647629.0/1978049680, -160528059.0/685178525, 248638103.0/1413531060, 0}
        } };
std::array<double, 12> butchers_b = {13451932.0/455176623, 0, 0, 0, 0,
                                    -808719846.0/976000145, 1757004468.0/5645159321,
                                    656045339.0/265891186, -3867574721.0/1518517206,
                                    465885868.0/322736535, 53011238.0/667516719,
                                    2.0/45};

std::array<double, 13> butchers_bs = {14005451.0/335480064, 0, 0, 0, 0,
                                      -59238493.0/1068277825, 181606767.0/758867731,
                                      561292985.0/797845732, -1041891430.0/1371343529,
                                      760417239.0/1151165299, 118820643.0/751138087,
                                      -528747749.0/2220607170, 1.0/4};

std::array<double, 13> butchers_c = { 0, 1.0/18, 1.0/12, 1.0/8, 5.0/16, 3.0/8,
                                    59.0/400, 93.0/200, 5490023248.0/9719169821,
                                    13.0/20, 1201146811.0/1299019798, 1, 1};





// Функция правых частей
std::array<double, 6> function_of_right_values(double time, 
    const std::array<double, 6>& rv)
{
    using namespace dph;



    double mus[8] = {  // Гравитационные постоянные
        22032,          // Меркурий
        324859,         // Венера
        //398600.4415,  // Земля
        42828,          // Марс
        126686534,      // Юпитер
        37931187,       // Сатурн
        5793939,        // Уран
        6836529,        // Нептун
        132712440018    // Солнце
    };


    constexpr unsigned indices[8] = { // Значения параметров для метода dph::EphemerisReelase::calculateBody
        B_MERCURY,
        B_VENUS,
        //B_EARTH,
        B_MARS,
        B_JUPITER,
        B_SATURN,
        B_URANUS,
        B_NEPTUNE,
        B_SUN
    };




    double mu_earth = 398600.4415;

    double rv_mod = vec_mod(rv.data());
    double rv_mod3 = rv_mod * rv_mod * rv_mod;

    std::array<double, 3> a;
    for (int i = 0; i < 3; i++)
        a[i] = -mu_earth * rv[i] / rv_mod3;



    // Возмущения от планет и солнца
    for (int i = 0; i < 8; i++)
    {
        double mu_body = mus[i];

        double body[3];
        DE421.calculateBody(CALC_POS, indices[i], B_EARTH, mjd2jd(time), body);

        double body_mod = vec_mod(body);
        double body_mod3 = body_mod * body_mod * body_mod;

        double d_body_rv[3] {(body[0] - rv[0]), (body[1] - rv[1]),
                    (body[2] - rv[2])};

        double d_body_rv_mod = vec_mod(d_body_rv);

        double d_body_rv_mod3 = d_body_rv_mod * d_body_rv_mod * d_body_rv_mod;

        for (int i = 0; i < 3; i++)
            a[i] += mu_body * (d_body_rv[i] / d_body_rv_mod3 - body[i] / body_mod3);
    }


    std::array<double, 6> answ = { rv[3], rv[4], rv[5], a[0], a[1], a[2] };
    return answ;
}


// Создание матрицы скосов
std::array<std::array<double, 6>, 13> make_slopes(double mjd,
    const std::array<double, 6>& init_state, double h_sec)
{
    std::array<std::array<double, 6>, 13> slopes;
    slopes[0] = function_of_right_values(mjd + butchers_c[0] * h_sec, init_state);
    for (int i = 1; i < 13; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            double h_koeff = 0;
            for (int k = 0; k < i; k++)
            {
                h_koeff = h_koeff + butchers_a[i - 1][k] * slopes[k][j];
            }
            slopes[i][j] = init_state[j] + h_sec * h_koeff;
        }
        slopes[i] = function_of_right_values(mjd + h_sec * butchers_c[i] / 86400,
            slopes[i]);
    }
    return slopes;
}


// Определение вектора состояния (фактического или оценочного)
std::array<double, 6> make_state(
        const std::array<std::array<double, 6>, 13>& slopes,
        const std::array<double, 6>& init_state, double h_sec, state_type type)
{
    std::array<double, 6> answer;

    double* b  = type == STRAIGHT ? butchers_b.data() : butchers_bs.data();
    int b_size = type == STRAIGHT ? 12 : 13;

    for (int j = 0; j < 6; j++)
    {
        double ks = 0;
        for (int i = 0; i < b_size; i++)
        {
            ks = ks + b[i] * slopes[i][j];
        }
        answer[j] = init_state[j] + h_sec * ks;

    }

    return answer;
}


// Реализация метода Рунге-Кутта с помощью Таблицы Бутчера с заданным шагом
void dopri8_fixed(double& mjd, std::array<double, 6>& state, double h_sec)
{
    auto slopes = make_slopes(mjd, state, h_sec);
    state = make_state(slopes, state, h_sec, STRAIGHT);
    mjd += h_sec / 86400;
}


// Реализация метода Рунге-Кутта с динамическим определением шага по времени
void dopri8_auto(double mjd, std::array<double, 6>& state, double& h_sec)
{
    int j = 0;
    while (true)
    {
        auto slopes = make_slopes(mjd, state, h_sec);
        auto state_b = make_state(slopes, state, h_sec, STRAIGHT);
        auto state_bs = make_state(slopes, state, h_sec, ESTIMATED);
//        double time_copy = mjd;
//        dopri8_fixed(time_copy, state_b, h_sec);
//        dopri8_fixed_bs(time_copy, state_bs, h_sec);

        double d[3];
        for (int i = 0; i < 3; i++)
            d[i] = fabs(state_b[i] - state_bs[i]);

        double err = *std::max_element(d, d + 3);

        j++;
        if (err > 1e-8)
        {
            h_sec *= 0.7;
        }
        else if (err < 1e-12)
        {
            h_sec *= 1.4;
        }
        else
        {
            state = state_b;
            break;
        }
    }
}


// Обертка для метода Рунге-Кутта с заданным интервальным значением времени
void dopri8(double& mjd, std::array<double, 6>& state, double interval,
    CallBack callBack)
{
    double step_sum = 0;
    double step_sec = 1;
    std::array<double, 6> state_local_copy = state;

    if (callBack)
        callBack(mjd, state);

    while (true)
    {
        double currentMjd = mjd + step_sum / 86400;
        dopri8_auto(currentMjd, state_local_copy, step_sec);
        if (step_sum + step_sec > interval)
            break;

        step_sum += step_sec;
        state = state_local_copy;

        if (callBack)
            callBack(mjd + step_sum / 86400, state);
    }

    if (interval - step_sum > 0)
    {
        mjd += step_sum / 86400;
        dopri8_fixed(mjd, state, interval - step_sum);

        if (callBack)
            callBack(mjd, state);
    }
}



// Обертка для dopri8 для получения состояния через заданный шаг
void dopri8(double mjd, std::array<double, 6> state, double interval,
    double h_sec, CallBack callBack)
{
    if (callBack)
        callBack(mjd, state);

    size_t count = interval / h_sec;

    for(size_t i = 0; i < count; i++)
    {
        dopri8(mjd, state, h_sec);
        if (callBack)
            callBack(mjd, state);
    }

}











