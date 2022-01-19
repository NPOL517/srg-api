#include <iostream>

int main()
{
    std::cout << "SANDBOX" << std::endl;
    double a = 0.344965;
    int msecs = 86400000 * a + 43200000; // Число миллисекунд, прошедшее с начала суток указанного дня
    int b = (msecs % 3600000);
    int hours = (msecs / 3600000); // Часы
    int minutes = b / 60000; // Минуты
    int seconds = (b % 60000) / 1000; //Секунды
    int mseconds = (b % 60000) % 1000;
    
    int as = 5;
    double ba = 5.5;
    std::cout << as - ba;
    std::cout << "\n" << hours << "\n" << minutes << "\n" << seconds << "\n" << mseconds;
    return 0;
}



// 1275 минут
// 1275 div 60