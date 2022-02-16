#ifndef RK4_H
#define RK4_H
#include <array>

// �����-����� 4 ������� ��� ������ 
void rk_sin(double& x_0, double& y_0, double h);

// ������� ������ ������
std::array<double, 6> function_of_right_values(std::array<double, 6>& arr);

// �����-����� 4 ������� ��� ������ ���� ���
std::array<double, 6> rk4(double& time, std::array<double, 6>& arr_baz, double h);

#endif // RK4_H