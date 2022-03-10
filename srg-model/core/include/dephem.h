#ifndef DEPHEM_HPP
#define DEPHEM_HPP

/*******************************************************************************

      8888888b.  8888888888 8888888b.  888    888 8888888888 888b     d888
      888  "Y88b 888        888   Y88b 888    888 888        8888b   d8888
      888    888 888        888    888 888    888 888        88888b.d88888
      888    888 8888888    888   d88P 8888888888 8888888    888Y88888P888
      888    888 888        8888888P"  888    888 888        888 Y888P 888
      888    888 888        888        888    888 888        888  Y8P  888
      888  .d88P 888        888        888    888 888        888   "   888
      8888888P"  8888888888 888        888    888 8888888888 888       888

                  С++11 БИБЛИОТЕКА ДЛЯ РАБОТЫ С DE-ЭФЕМЕРИДАМИ
                   ВЕРСИЯ 0.4 [DEVELOP/UNPUBLISHED/SRG-API]

*******************************************************************************/

#include <fstream>
#include <cstring>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

namespace dph {

/* -----------------------------| HELP VALUES |------------------------------ */

/* Значения параметров для метода dph::EphemerisReelase::calculateBody(...).
   Данные значения соответствуют индексам тел, для которых можно получить
   результат вычислений. */
constexpr unsigned B_MERCURY = 1;
constexpr unsigned B_VENUS   = 2;
constexpr unsigned B_EARTH   = 3;
constexpr unsigned B_MARS    = 4;
constexpr unsigned B_JUPITER = 5;
constexpr unsigned B_SATURN  = 6;
constexpr unsigned B_URANUS  = 7;
constexpr unsigned B_NEPTUNE = 8;
constexpr unsigned B_PLUTO   = 9;
constexpr unsigned B_MOON    = 10;
constexpr unsigned B_SUN     = 11;
constexpr unsigned B_SSBARY  = 12; /* Барицентр Солнечной Системы. */
constexpr unsigned B_EMBARY  = 13; /* Барицентр системы Земля-Луна. */

/* Значения параметров для метода dph::EphemerisRelease::calculateOther(...).
   Данные значения соответствуют индексам прочих элементов, для которых можно
   получить результат вычислений. */
constexpr unsigned O_EARTH_NUTATIONS               = 14;
constexpr unsigned O_LUNAR_MANTLE_LIBRATION        = 15;
constexpr unsigned O_LUNAR_MANTLE_ANGULAR_VELOCITY = 16;
constexpr unsigned O_TTmTDB                        = 17;

/* Значения параметров для методов dph::EphemerisRelease::calculateBody(...) и
   dph::EphemerisRelease::calculateOther(...).
   Данные значения соответствуют индексам результатов вычислений, которые можно
   получить. */
constexpr unsigned CALC_POS   = 0;
constexpr unsigned CALC_STATE = 1;

/* -------------------------------------------------------------------------- */


/* -----------------------| CLASS EPHEMERIS_RELEASE |------------------------ */
class EphemerisRelease
{
public:

    /* [СТАНДАРТНЫЕ МЕТОДЫ] */

    /* Конструктор по умолчанию. */
    EphemerisRelease();

    /* Конструктор по пути к бинарному файлу эфемерид.
            Чтение файла, проверка полученных значений. */
    explicit EphemerisRelease(const std::string& binaryFilePath);

    /* Конструктор копирования.
            Проверка полученных значений и доступ к файлу.
            При неудачной проверке объект очищается. */
    EphemerisRelease(const EphemerisRelease& other);

    /* Оператор копирования.
            Проверка полученных значений и доступ к файлу.
            При неудачной проверке объект очищается. */
    EphemerisRelease& operator=(const EphemerisRelease& other);

    /* Деструктор.
            Просто деструктор.*/
    ~EphemerisRelease();


    /* [ПОЛЬЗОВАТЕЛЬСКИЕ МЕТОДЫ] */

    /* Открыть бинарый файл эфемерид. */
    bool open(const std::string& binaryFilePath);

    /* Открыть бинарный файл эфемерид. */
    bool open(const std::string& binaryFileName,
        const std::vector<std::string>& searchDirs);

    /* Радиус-вектор (или вектор состояния) одного тела относительно другого. */
    void calculateBody(unsigned calculationResult, unsigned targetBody,
        unsigned centerBody, double JED, double* resultArray) const;

    /* Значение одного из дополнительных элементов, хранящихся в выпуске DE. */
    void calculateOther(unsigned calculationResult, unsigned otherItem,
        double JED, double* resultArray) const;

    /* Готовность объекта к использованию. */
    bool isReady() const;

    /* Первая доступная дата для рассчётов. */
    double startDate() const;

    /* Последняя доступная дата для рассчётов. */
    double endDate() const;

    /* Номер выпуска. */
    uint32_t releaseIndex() const;

    /* Заголовочная информация DE. */
    const std::string& releaseLabel() const;

    /* Значение константы по её имени. */
    double constant(const std::string& constantName) const;


private:

    /* [ПРИВАТНЫЕ КОНСТАНТЫ И ПЕРМЕННЫЕ] */

    /* Формат DE-эфемерид. */
    static const size_t RLS_LABELS_COUNT = 3;    // Число строк в метке выпуска.
    static const size_t RLS_LABEL_SIZE   = 84;   // Длина строки метки.
    static const size_t CNAME_SIZE       = 6;    // Длина имени константы.
    static const size_t CCOUNT_MAX_OLD   = 400;  // Кол-во констант (стар.).
    static const size_t CCOUNT_MAX_NEW   = 1000; // Кол-во констант (нов.).

    /* Заголовочная информация выпуска DE. */
    std::string m_releaseLabel;    // Метка выпуска.
    uint32_t    m_releaseIndex{};  // Номерная часть индекса выпуска.
    double      m_startDate{};     // Дата начала выпуска (JED).
    double      m_endDate{};       // Дата окончания выпуска (JED).
    double      m_blockTimeSpan{}; // Временная протяжённость блока.
    uint32_t    m_keys[15][3]{};   // Ключи поиска коэффициентов.
    double      m_au{};            // Астрономическая единица [км].
    double      m_emrat{};         // Отношение массы Земли к массе Луны.
    std::map<std::string, double> m_constants; // Константы выпуска.

    /* Производные значения для работы с выпуском DE. */
    size_t   m_blocksCount{};     // Количество блоков в файле.
    uint32_t m_ncoeff{};          // Количество коэффициентов в блоке.
    double   m_emrat2{};          // Отношение массы Луны к массе Земля-Луна.
    double   m_dimensionFit{};    // Значение для соблюдения размерности.
    size_t   m_blockSize_bytes{}; // Размер блока в байтах.

    /* Обеспечение работоспособности объекта. */
    bool m_ready{};                           // Готовность объекта к работе.
    std::string m_binaryFilePath;             // Путь к файлу эфемерид.
    mutable std::ifstream m_binaryFileStream; // Поток чтения файла.
    mutable std::vector<double> m_buffer;	  // Буффер блока с коэффициентами.
    mutable std::vector<double> m_poly;		  // Значения полиномов.
    mutable std::vector<double> m_dpoly;	  // Значения производных полиномов.


    /* [ПРИВАТНЫЕ МЕТОДЫ] */

    /* Обрезать повторяющиеся пробелы (' ') с конца массива символов "charArray"
       размера "arraySize". */
    static std::string cutBackSpaces(const char* charArray, size_t arraySize);

    /* Приведение объекта к изначальному состоянию. */
    void clear();

    /* Копирование информации из объекта "other" в текущий объект. */
    void copyHere(const EphemerisRelease& other);


    /* [Работа с файлом] */

    /* Считать данные из бинарного файла выпуска DE в объект. */
    void readAndPackData();

    /* Дополнительные вычисления после чтения файла.
       Входит в состав readAndPackData(). */
    void additionalCalculations();

    /* Проверка значений, хранящихся в объекте и проверка файла. */
    bool isDataCorrect() const;

    /* Проверка начальных и конечных дат всех блоков в файле.
       Подтверждает целостность файла и доступность всех коэффициентов.
       Входит в состав проверки isDataCorrect(). */
    bool check_blocksDates() const;

    /* Заполнение буффера "m_buffer" коэффициентами требуемого блока. */
    void fillBuffer(size_t block_num) const;


    /* [Вычисления] */

    /* Интерполяция компонент базового элемента. */
    void interpolatePosition(unsigned baseItemIndex, double normalizedTime,
        const double* coeffArray, unsigned componentsCount,
        double* resultArray) const;

    /* Интерполяция компонент и их первых производных базового элемента. */
    void interpolateState(unsigned baseItemIndex, double normalizedTime,
        const double* coeffArray, unsigned componentsCount,
        double* resultArray) const;

    /* Значения компонент базового элемента. */
    void calculateBaseItem(unsigned baseItemIndex, double JED,
        unsigned calculationResult , double* resultArray) const;

    /* Радиус-вектор (или вектор состояния) Земли относительно барицентра СС. */
    void calculateBaseEarth(double JED, unsigned calculationResult,
        double* resultArray) const;

    /* Радиус-вектор (или вектор состояния) Луны относительно барицентра СС. */
    void calculateBaseMoon(double JED, unsigned calculationResult,
        double* resultArray) const;

}; /* class EphemerisRelease */
/* -------------------------------------------------------------------------- */

} /* namespace dph */

#endif // DEPHEM_HPP
