#include <iostream>

int main()
{
    std::cout << "SANDBOX" << std::endl;
    double a = 0.344965;
    int msecs = 86400000 * a + 43200000; // ����� �����������, ��������� � ������ ����� ���������� ���
    int b = (msecs % 3600000);
    int hours = (msecs / 3600000); // ����
    int minutes = b / 60000; // ������
    int seconds = (b % 60000) / 1000; //�������
    int mseconds = (b % 60000) % 1000;
    
    int as = 5;
    double ba = 5.5;
    std::cout << as - ba;
    std::cout << "\n" << hours << "\n" << minutes << "\n" << seconds << "\n" << mseconds;
    return 0;
}



// 1275 �����
// 1275 div 60