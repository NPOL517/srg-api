#include "dephem.h"

/* Конструктор по умолчанию. */
dph::EphemerisRelease::EphemerisRelease()
{
    /* Инициализация внутренних переменных. */
    this->clear();
}

/* Конструктор по пути к бинарному файлу эфемерид.
            Чтение файла, проверка полученных значений. */
dph::EphemerisRelease::EphemerisRelease(const std::string& binaryFilePath)
{
    /* Открыть бинарый файл эфемерид. */
    this->open(binaryFilePath);
}

/* Конструктор копирования.
            Проверка полученных значений и доступ к файлу.
            При неудачной проверке объект очищается. */
dph::EphemerisRelease::EphemerisRelease(const EphemerisRelease& other)
{
    if (other.m_ready)
    {
        copyHere(other);

        if (isDataCorrect())
        {
            m_ready = true;
        }
        else
        {
            m_ready = false;

            clear();
        }
    }
}

/* Оператор копирования.
            Проверка полученных значений и доступ к файлу.
            При неудачной проверке объект очищается. */
dph::EphemerisRelease&
dph::EphemerisRelease::operator=(const EphemerisRelease& other)
{
    if (other.m_ready)
    {
        clear();
        copyHere(other);

        if (isDataCorrect())
        {
            m_ready = true;
        }
        else
        {
            m_ready = false;

            clear();
        }
    }

    return *this;
}

/* Деструктор.
            Просто деструктор.*/
dph::EphemerisRelease::~EphemerisRelease()
{
    m_binaryFileStream.close();
}

/* Открыть бинарый файл эфемерид. */
bool dph::EphemerisRelease::open(const std::string& binaryFilePath)
{
    /* Инициализация внутренних переменных. */
    clear();

    /* Копирование пути к файлу. */
    m_binaryFilePath = binaryFilePath;

    /* Открытие файла. */
    m_binaryFileStream.open(m_binaryFilePath.c_str(), std::ios::binary);

    if (!m_binaryFileStream.is_open())
    {
        m_binaryFilePath.clear();
        return false;
    }

    readAndPackData();

    if (!isDataCorrect())
    {
        clear();
        return false;
    }

    m_ready = true;
    return true;
}

/* Открыть бинарый файл эфемерид. */
bool dph::EphemerisRelease::open(const std::string& binaryFileName,
    const std::vector<std::string>& searchDirs)
{
    for (auto& dir : searchDirs)
    {
        auto path = dir.empty() ? binaryFileName : dir + "/" + binaryFileName;

        if (open(path))
        {
            return true;
        }
    }

    return false;
}


/* Радиус-вектор (или вектор состояния) одного тела относительно другого. */
void dph::EphemerisRelease::calculateBody(unsigned calculationResult,
    unsigned targetBody, unsigned centerBody, double JED,
    double* resultArray) const
{
    /*
     *  Параметры:
     *   calculationResult – Индекс результата вычислений.
     *   targetBody        – Порядковый номер искомого тела.
     *   centerBody        – Порядковый номер центрального тела.
     *   JED               – Момент времени (Julian Epehemris Date).
     *   resultArray       – Результат вычислений.
     *
     *  Примечания:
     *    – Если в метод поданы неверные параметры, то он просто прервётся.
     */

    /*
     *  Допустимые значения параметров
     *
     *  calculationResult:
     *		1 - Получить значение радиус-вектора,
     *		2 - Получить значение вектора состояния
     *
     *   targetBody, centerBody:
     *      1  – Меркурий;
     *	    2  – Венера;
     *		3  – Земля;
     *		4  – Марс;
     *		5  – Юпитер;
     *		6  – Сатурн;
     *		7  – Уран;
     *		8  – Нептун;
     *		9  – Плутон;
     *		10 – Луна;
     *		11 – Солнце;
     *		12 – Барицентр Солнечной Системы;
     *		13 – Барицентр системы Земля-Луна;
     *
     *  JED:
     *      JED должен принадлежать промежутку: [m_startDate : m_endDate].
     *
     *  resultArray:
     *      – Размер массива:
     *          3 при calculationResult = 0;
     *          6 при calculationResult = 1.
     *      – Не должен быть нулевым указателем.
     */

    /* Проверка переданных параметров. */
    if (this->m_ready == false)
    {
        return;
    }
    else if (calculationResult > 1)
    {
        return;
    }
    else if (targetBody == 0 || centerBody == 0)
    {
        return;
    }
    else if (targetBody > 13 || centerBody > 13)
    {
        return;
    }
    else if (JED < m_startDate || JED > m_endDate)
    {
        return;
    }
    else if (resultArray == nullptr)
    {
        return;
    }

    /* Количество требуемых компонент. */
    unsigned componentsCount = (calculationResult == CALC_STATE) ? 6 : 3;

    /* Выбор методики вычисления в зависимости от комбинации искомого и
       центрального тела. */
    if (targetBody == centerBody)
    {
        /* Случай #1: Искомое тело является центральным.
                      Результатом является нулевой вектор.*/

        /* Заполнение массива нулями */
        std::memset(resultArray, 0, sizeof(double) * componentsCount);
    }
    else if (targetBody == B_SSBARY || centerBody == B_SSBARY)
    {
        /* Случай #2: Искомым или центральным телом является барицентр СС.
                      Так как все методы calculateBase для тел возвращают вектор
                      относительно барцентра СС, то достаточно вычислить
                      пололжение второго тела. В случае, если искомым телом
                      является сам барицентр СС, то возвращается "зеркальный"
                      вектор второго тела. */

        /* Индекс тела, что не является барицентром СС. */
        unsigned notSSBARY = targetBody == B_SSBARY ? centerBody : targetBody;

        /* Выбор метода вычисления в зависимости от тела. */
        switch (notSSBARY)
        {
            case B_EARTH:
                calculateBaseEarth(JED, calculationResult, resultArray);
                break;

            case B_MOON:
                calculateBaseMoon(JED, calculationResult, resultArray);
                break;

            case B_EMBARY:
                calculateBaseItem(2, JED, calculationResult, resultArray);
                break;

            default:
                calculateBaseItem(notSSBARY - 1, JED, calculationResult,
                    resultArray);
        }

        /* Если барицентр СС является искомым телом, то возвращается
           "зеркальный" вектор. */
        if (targetBody == B_SSBARY)
        {
            for (unsigned i = 0; i < componentsCount; ++i)
            {
                resultArray[i] = -resultArray[i];
            }
        }
    }
    else if (targetBody * centerBody == 30 && targetBody + centerBody == 13)
    {
        /* Случай #3: Искомым и центральным телами являетса Земля и Луна.
                      В этом случае достаточно получить значение положения Луны
                      относительно Земли (базовый элемент #9 (от нуля).
                      В случае, если искомым телом является Земля, то
                      возвращается "зеркальный вектор" */

        /* Получение радиус-вектора (или вектора состояния) Луны относительно
           Земли. */
        calculateBaseItem(9, JED, calculationResult, resultArray);

        /* Если искомым телом является Земля, то возвращается "зеркальный"
           вектор. */
        if (targetBody == B_EARTH)
        {
            for (unsigned i = 0; i < componentsCount; ++i)
            {
                resultArray[i] = -resultArray[i];
            }
        }
    }
    else
    {
        /* Случай #4: Все остальные комбинации тел.
                      Сначала вычисляется значение центрального тела
                      относительно барицентра СС, после - искомого. Результатом
                      является разница между вектором центрального тела и
                      искомого. */

        /* Массив для центрального тела. */
        double centerBodyArray[6];

        /* Две итерации. */
        for (unsigned i = 0; i <= 1; ++i)
        {
            /* Определение индекса и массива в зависимости от номера итерации.
               i == 0 : работа с центральным телом.
               i == 1 : работа с искомым телом. */
            unsigned currentBodyIndex = i == 0 ? centerBody : targetBody;
            double* currentArray = i == 0 ? centerBodyArray : resultArray;

            /* Выбор метода вычисления в зависимости от тела. */
            switch (currentBodyIndex)
            {
                case B_EARTH:
                    calculateBaseEarth(JED, calculationResult, currentArray);
                    break;

                case B_MOON:
                    calculateBaseMoon(JED, calculationResult, currentArray);
                    break;

                case B_EMBARY:
                    calculateBaseItem(2, JED, calculationResult, currentArray);
                    break;

                default:
                    calculateBaseItem(currentBodyIndex - 1, JED,
                        calculationResult, currentArray);
            }
        }

        /* Разница между вектором центрального и искомого тела. */
        for (unsigned i = 0; i < componentsCount; ++i)
        {
            resultArray[i] -= centerBodyArray[i];
        }
    }
}

/* Значение одного из дополнительных элементов, хранящихся в выпуске DE. */
void dph::EphemerisRelease::calculateOther(unsigned calculationResult,
    unsigned otherItem, double JED, double* resultArray) const
{
    /*
     *  Параметры:
     *   calculationResult – Индекс результата вычислений.
     *   otherItem         – Порядковый номер искомого элемента
     *   JED               – Момент времени (Julian Epehemris Date).
     *   resultArray       – Результат вычислений.
     *
     *  Примечания:
     *    – Если в метод поданы неверные параметры, то он просто прервётся.
     *    – Не всегда в выпуске эфемерид хранится запрашиваемое тело, убедитесь
     *      в его наличии перед запросом.
     */

    /*
     *  Допустимые значения параметров
     *
     *   calculationResult:
     *		1 - Получить значение радиус-вектора,
     *		2 - Получить значение вектора состояния
     *
     *   otherItem:
     *      14 – Земные нутации в долдготе и наклоне (модель IAU 1980);
     *      15 – Либрации лунной мантии;
     *		16 – Угловые скорости лунной мантии;
     *		17 – TT-TDB (в центре Земли).
     *
     *  JED:
     *      JED должен принадлежать промежутку: [m_startDate : m_endDate].
     *
     *  resultArray:
     *      – Размер массива в зависимости от результата вычислений и искомого
     *        элемента.
     *      – Не должен быть нулевым указателем.
     */

    /* Проверка переданных параметров. */
    if (this->m_ready == false)
    {
        return;
    }
    else if (calculationResult > 1)
    {
        return;
    }
    else if (otherItem < 14 || otherItem > 17)
    {
        return;
    }
    else if (JED < m_startDate || JED > m_endDate)
    {
        return;
    }
    else if (resultArray == nullptr)
    {
        return;
    }
    else
    {
        calculateBaseItem(otherItem - 3, JED, calculationResult, resultArray);
    }
}


/* Готовность объекта к использованию. */
bool dph::EphemerisRelease::isReady() const
{
    return m_ready;
}

/* Первая доступная дата для рассчётов. */
double dph::EphemerisRelease::startDate() const
{
    return m_startDate;
}

/* Последняя доступная дата для рассчётов. */
double dph::EphemerisRelease::endDate() const
{
    return m_endDate;
}

/* Номер выпуска. */
uint32_t dph::EphemerisRelease::releaseIndex() const
{
    return m_releaseIndex;
}

/* Заголовочная информация DE. */
const std::string& dph::EphemerisRelease::releaseLabel() const
{
    return m_releaseLabel;
}

/* Значение константы по её имени. */
double dph::EphemerisRelease::constant(const std::string& constantName) const
{
    if (m_ready == false)
    {
        return 0.0;
    }
    else if (constantName == "AU")
    {
        return m_au;
    }
    else if (constantName == "EMRAT")
    {
        return m_emrat;
    }
    else if (constantName == "DENUM")
    {
        return m_releaseIndex;
    }
    else
    {
        return m_constants.find(constantName)->second;
    }
}

/* Обрезать повторяющиеся пробелы (' ') с конца массива символов "charArray"
       размера "arraySize". */
std::string dph::EphemerisRelease::cutBackSpaces(const char* charArray,
    size_t arraySize)
{
    for (size_t i = arraySize - 1; i > 0; --i)
    {
        if (charArray[i] == ' ' && charArray[i - 1] != ' ')
        {
            return std::string(charArray, i);
        }
    }

    return std::string(charArray, arraySize);
}

/* Приведение объекта к изначальному состоянию. */
void dph::EphemerisRelease::clear()
{
    m_ready = false;

    m_binaryFilePath.clear();
    m_binaryFileStream.close();

    m_releaseLabel.clear();
    m_releaseIndex = 0;
    m_startDate = 0.0;
    m_endDate = 0.0;
    m_blockTimeSpan = 0.0;
    std::memset(m_keys, 0, sizeof(m_keys));
    m_au = 0.0;
    m_emrat = 0.0;
    std::map<std::string, double>().swap(m_constants);

    m_blocksCount = 0;
    m_ncoeff = 0;
    m_dimensionFit = 0;
    m_blockSize_bytes = 0;

    std::vector<double>().swap(m_buffer);
    std::vector<double>(1).swap(m_poly);
    std::vector<double>(2).swap(m_dpoly);

    m_poly[0]  = 1;
    m_dpoly[0] = 0;
    m_dpoly[1] = 1;
}

/* Копирование информации из объекта "other" в текущий объект. */
void dph::EphemerisRelease::copyHere(const EphemerisRelease& other)
{
    /* Используется в:
        - Конструкторе копирования.
        - Операторе копирования. */

    m_ready = other.m_ready;

    m_binaryFilePath	= other.m_binaryFilePath;

    m_binaryFileStream.close();
    m_binaryFileStream.open(other.m_binaryFilePath.c_str(), std::ios::binary);

    m_releaseLabel =	other.m_releaseLabel;
    m_releaseIndex =	other.m_releaseIndex;
    m_startDate =		other.m_startDate;
    m_endDate =			other.m_endDate;
    m_blockTimeSpan =	other.m_blockTimeSpan;
    std::memcpy(m_keys, other.m_keys, sizeof(m_keys));
    m_au =				other.m_au;
    m_emrat =			other.m_emrat;
    m_constants =		other.m_constants;

    m_blocksCount =		other.m_blocksCount;
    m_ncoeff =			other.m_ncoeff;
    m_emrat2 =			other.m_emrat2;
    m_dimensionFit =	other.m_dimensionFit;
    m_blockSize_bytes = other.m_blockSize_bytes;

    m_buffer =	other.m_buffer;
    m_poly =	other.m_poly;
    m_dpoly =	other.m_poly;
}

/* Считать данные из бинарного файла выпуска DE в объект. */
void dph::EphemerisRelease::readAndPackData()
{
    /* Строковая информация о выпуске DE (метка). */
    char releaseLabel_buffer[RLS_LABELS_COUNT][RLS_LABEL_SIZE];

    /* Имена констант. */
    char constantsNames_buffer[CCOUNT_MAX_NEW][CNAME_SIZE];

    /* Значения констант. */
    double constantsValues_buffer[CCOUNT_MAX_NEW];

    /* Количество констант в файле эфемерид. */
    uint32_t constantsCount;


    /* [Чтение файла] */

    m_binaryFileStream.seekg(0, std::ios::beg);
    m_binaryFileStream.read(reinterpret_cast<char*>(&releaseLabel_buffer),
                            RLS_LABEL_SIZE * RLS_LABELS_COUNT);
    m_binaryFileStream.read(reinterpret_cast<char*>(&constantsNames_buffer),
                            CNAME_SIZE * CCOUNT_MAX_OLD);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_startDate), 8);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_endDate), 8);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_blockTimeSpan), 8);
    m_binaryFileStream.read(reinterpret_cast<char*>(&constantsCount), 4);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_au), 8);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_emrat), 8);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_keys), (12 * 3) * 4);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_releaseIndex), 4);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_keys[12]), (3) * 4);

    /* Чтение дополнительных констант. */
    if (constantsCount > 400)
    {
        /* Количество дополнительных констант. */
        int extraConstantsCount = constantsCount - CCOUNT_MAX_OLD;
        int bufsize = extraConstantsCount * static_cast<int>(CNAME_SIZE);

        m_binaryFileStream.read(
            reinterpret_cast<char*>(&constantsNames_buffer[CCOUNT_MAX_OLD]),
            bufsize);
    }

    /* Чтение дополнительных ключей. */
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_keys[13]), (3 * 2) * 4);

    /* Подсчёт ncoeff (количество коэффициентов в блоке). */
    m_ncoeff = 2;
    for (int i = 0; i < 15; ++i)
    {
        /* Количество компонент для выбранного элемента. */
        int comp = i == 11 ? 2 : i == 14 ? 1 : 3;
        m_ncoeff += static_cast<uint32_t>(comp) * m_keys[i][1] * m_keys[i][2];
    }

    /* Переход к блоку с константами и их чтение. */
    if (constantsCount <= CCOUNT_MAX_NEW)
    {
        int offset  = static_cast<int>(m_ncoeff * 8);
        int bufsize = static_cast<int>(constantsCount * 8);
        m_binaryFileStream.seekg(offset, std::ios::beg);
        m_binaryFileStream.read(
            reinterpret_cast<char*>(&constantsValues_buffer), bufsize);
    }


    /* [Форматирование и упаковка считанной информации] */

    /* Формирование строк метки выпуска DE. */
    for (size_t i = 0; i < RLS_LABELS_COUNT; ++i)
    {
        m_releaseLabel += cutBackSpaces(releaseLabel_buffer[i], RLS_LABEL_SIZE);
        m_releaseLabel += '\n';
    }

    /* Заполнение контейнера m_constants именами и значениями констант. */
    if (constantsCount > 0 && constantsCount <= CCOUNT_MAX_NEW)
    {
        for (uint32_t i = 0; i < constantsCount; ++i)
        {
            std::string constantName = cutBackSpaces(constantsNames_buffer[i],
                CNAME_SIZE);
            m_constants[constantName] = constantsValues_buffer[i];
        }
    }

    /* Дополнительные вычисления. */
    additionalCalculations();
}

/* Дополнительные вычисления после чтения файла.
   Входит в состав readAndPackData(). */
void dph::EphemerisRelease::additionalCalculations()
{
    /* Дополнительные коэффициенты. */
    m_emrat2 = 1.0 / (1 + m_emrat);
    m_dimensionFit = 1.0 / (43200.0 * m_blockTimeSpan);

    /* Количество блоков с коэффициентами. */
    m_blocksCount = size_t((m_endDate - m_startDate) / m_blockTimeSpan);

    /* Максимальная степень интерполяционного полинома. */
    size_t maxPolynomsCount = 0;
    for (int i = 0; i < 15; ++i)
    {
        if (m_keys[i][1] > maxPolynomsCount)
        {
            maxPolynomsCount = m_keys[i][1];
        }
    }

    /* Размер блока в байтах. */
    m_blockSize_bytes = m_ncoeff * sizeof(double);

    /* Резервирование памяти. */
    m_buffer.resize(m_ncoeff);
    m_poly.resize(maxPolynomsCount);
    m_dpoly.resize(maxPolynomsCount);
}

/* Проверка значений, хранящихся в объекте и проверка файла. */
bool dph::EphemerisRelease::isDataCorrect() const
{
    /* В данном методе проверяются только те параметры, которые могут повлиять
       непосредственно на вычисления значений элементов, хранящихся в выпуске
       эфемерид. */

    if (m_binaryFileStream.is_open() == false)       return false;
    if (m_startDate >= m_endDate)                    return false;
    if (m_blockTimeSpan == 0)                        return false;
    if ((m_endDate - m_startDate) < m_blockTimeSpan) return false;
    if (m_emrat == 0)                                return false;
    if (m_ncoeff == 0)                               return false;
    if (check_blocksDates() == false)                return false;

    return true;
}


/* Проверка начальных и конечных дат всех блоков в файле.
   Подтверждает целостность файла и доступность всех коэффициентов.
   Входит в состав проверки isDataCorrect(). */
bool dph::EphemerisRelease::check_blocksDates() const
{
    /* Адрес первого блока с коэффициентами в файле. */
    size_t firstBlockAdress = m_blockSize_bytes * 2;

    /* Переход к первому блоку. */
    m_binaryFileStream.seekg(static_cast<int>(firstBlockAdress), std::ios::beg);

    /* Смещение между блоками после чтения двух первых коэффициентов. */
    size_t subBlockOffset = (m_ncoeff - 2) * sizeof(double);

    for (size_t blockIndex = 0; blockIndex < m_blocksCount; ++blockIndex)
    {
        /* Массив для чтения первых двух коэффициентов из текущего блока. */
        double blockDates[2] = {0.0, 0.0};

        /* Чтение. */
        m_binaryFileStream.read(reinterpret_cast<char*>(& blockDates),
            sizeof(blockDates));

        /* Значения, которые должны быть. */
        double blockStartDate = m_startDate + blockIndex * m_blockTimeSpan;
        double blockEndDate = blockStartDate + m_blockTimeSpan;

        if (blockDates[0] != blockStartDate || blockDates[1] != blockEndDate)
        {
            return false;
        }

        /* Переход к следующему блоку. */
        m_binaryFileStream.seekg(static_cast<int>(subBlockOffset),
            std::ios::cur);
    }

    return true;
}

/* Заполнение буффера "m_buffer" коэффициентами требуемого блока. */
void dph::EphemerisRelease::fillBuffer(size_t block_num) const
{
    size_t adress  = (2 + block_num) * m_blockSize_bytes;
    int    bufsize = static_cast<int>(m_ncoeff * 8);

    m_binaryFileStream.seekg(static_cast<int>(adress), std::ios::beg);
    m_binaryFileStream.read(reinterpret_cast<char*>(&m_buffer[0]),
        bufsize);
}

/* Интерполяция компонент базового элемента. */
void dph::EphemerisRelease::interpolatePosition(unsigned baseItemIndex,
    double normalizedTime, const double* coeffArray, unsigned componentsCount,
    double* resultArray) const
{
    /* Количество коэффициентов на компоненту. */
    uint32_t cpec = m_keys[baseItemIndex][1];

    /* Предварительное заполнение полиномов (вычисление их сумм). */
    m_poly[1] = normalizedTime;

    /* Заполнение полиномов (вычисление их сумм). */
    for (uint32_t i = 2; i < cpec; ++i)
    {
        m_poly[i] = 2 * normalizedTime * m_poly[i - 1] - m_poly[i - 2];
    }

    /* Обнуление массива результата вычислений. */
    memset(resultArray, 0, sizeof(double) * componentsCount);

    /* Вычисление координат. */
    for (unsigned i = 0; i < componentsCount; ++i)
    {
        for (uint32_t j = 0; j < cpec; ++j)
        {
            resultArray[i] += m_poly[j] * coeffArray[i * cpec + j];
        }
    }
}

/* Интерполяция компонент и их первых производных базового элемента. */
void dph::EphemerisRelease::interpolateState(unsigned baseItemIndex,
    double normalizedTime, const double* coeffArray, unsigned componentsCount,
    double* resultArray) const
{
    /* Количество коэффициентов на компоненту. */
    uint32_t cpec = m_keys[baseItemIndex][1];

    /* Предварительное заполнение полиномов (вычисление их сумм). */
    m_poly[1]  = normalizedTime;
    m_poly[2]  = 2 * normalizedTime * normalizedTime - 1;
    m_dpoly[2] = 4 * normalizedTime;

    /* Заполнение полиномов (вычисление их сумм). */
    for (uint32_t i = 3; i < cpec; ++i)
    {
        m_poly[i] = 2 * normalizedTime *  m_poly[i - 1] -  m_poly[i - 2];
        m_dpoly[i] = 2 * m_poly[i - 1] + 2 * normalizedTime * m_dpoly[i - 1] -
            m_dpoly[i - 2];
    }

    /* Обнуление массива результата вычислений. */
    memset(resultArray, 0, sizeof(double) * componentsCount * 2);

    /* Коэффициент для соблюдения размерности. */
    double derivative_units = m_keys[baseItemIndex][2] * m_dimensionFit;

    /* Вычисление координат. */
    for (unsigned i = 0; i < componentsCount; ++i)
    {
        for (uint32_t j = 0; j < cpec; ++j, ++coeffArray)
        {
            resultArray[i]                   +=  m_poly[j] * *coeffArray;
            resultArray[i + componentsCount] += m_dpoly[j] * *coeffArray;
        }

        resultArray[i + componentsCount] *= derivative_units;
    }
}

/* Значения компонент базового элемента. */
void dph::EphemerisRelease::calculateBaseItem(unsigned baseItemIndex,
    double JED, unsigned calculationResult, double* resultArray) const
{
    /*
     *  Параметры:
     *    baseItemIndex     – Индекс базового элемента выпуска DE (от нуля).
     *    JED               – Момент времени (Julian Epehemris Date).
     *    calculationResult – Индекс результата вычислений.
     *    resultArray       – Результат вычислений.
     */

    /*
     *  Допустимые значения параметров
     *
     *  baseItemIndex:
     *      0  – Меркурий;
     *      1  – Венера;
     *      2  – Барицентр системы Земля-Луна;
     *      3  – Марс;
     *      4  – Юпитер;
     *      5  – Сатурн;
     *      6  – Уран;
     *      7  – Нептун;
     *      8  – Плутон;
     *      9  – Луна (относительно Земли);
     *      10 – Солнце;
     *      11 – Земные нутации в долдготе и наклоне (модель IAU 1980);
     *      12 – Либрации лунной мантии;
     *      13 – Угловые скорости лунной мантии;
     *      14 – TT-TDB (в центре Земли).
     *
     *  JED:
     *      JED должен принадлежать промежутку: [m_startDate : m_endDate].
     *
     *  resultArray:
     *      – Размер массива в зависимости от результата вычислений и искомого
     *        элемента.
     *      – Не должен быть нулевым указателем.
     */

    /* Внимание!
       В ходе выполнения функции смысл переменных "normalizedTime" и "offset"
       будет меняться. */

    /* Норм. время относительно всех блоков в выпуске. */
    double normalizedTime = (JED - m_startDate) / m_blockTimeSpan;

    /* Порядковый номер блока, соотв. заданной дате JED
       (целая часть от normalizedTime). */
    size_t offset = static_cast<size_t>(normalizedTime);

    /* Заполнение буффера коэффициентами требуемого блока.
        Если требуемый блок уже в кэше объекта, то он не заполняется повторно.
        m_buffer[0] - дата начала блока.
        m_buffer[1] - дата окончания блока. */
    if (JED < m_buffer[0] || JED >= m_buffer[1])
    {
        /* Если JED равна последней доступоной дате для вычислений, то
           заполняется последний блок. */

        fillBuffer(offset - (JED == m_endDate ? 1 : 0));
    }

    if (JED == m_endDate)
    {
        /* Порядковый номер подблока (последний подблок). */
        offset = m_keys[baseItemIndex][2] - 1;

        /* Норм. время относительно подблока (в диапазоне от -1 до 1). */
        normalizedTime = 1;
    }
    else
    {
        /* Норм. время относительно всех подблоков. */
        normalizedTime = (normalizedTime - offset) * m_keys[baseItemIndex][2];

        /* Порядковый номер подблока (целая часть от normalizedTime). */
        offset = static_cast<size_t>(normalizedTime);

        /* Норм. время относительно подблока (в диапазоне от -1 до 1). */
        normalizedTime = 2 * (normalizedTime - offset) - 1;
    }

    /* Количество компонент для выбранного базового элемента. */
    unsigned componentsCount = (baseItemIndex == 11) ? 2 :
                               (baseItemIndex == 14) ? 1 : 3;

    /* Порядковый номер первого коэффициента в блоке. */
    size_t coeff_pos = m_keys[baseItemIndex][0] - 1 +
        componentsCount * offset * m_keys[baseItemIndex][1];

    /* Выбор метода вычисления в зависимости от заданного результата
       вычислений. */
    switch(calculationResult)
    {
    case CALC_POS :
        interpolatePosition(baseItemIndex, normalizedTime, &m_buffer[coeff_pos],
            componentsCount, resultArray);
        break;

    case CALC_STATE :
        interpolateState(baseItemIndex, normalizedTime, &m_buffer[coeff_pos],
            componentsCount, resultArray);
        break;

    default:
        memset(resultArray, 0, componentsCount * sizeof(double));
    }
}

/* Получить значение радиус-вектора (или вектора состояния) Земли
   относительно барицентра Солнечной Системы. */
void dph::EphemerisRelease::calculateBaseEarth(double JED, unsigned
    calculationResult, double* resultArray) const
{
    /* Радиус-вектор (или вектор состояния) барицентра сиситемы Земля-Луна
       относительно барицентра СС. */
    calculateBaseItem(2, JED, calculationResult, resultArray);

    /* Радиус-вектор (или вектор состояния) Луны относитльно Земли. */
    double MoonRelativeEarth[6];
    calculateBaseItem(9, JED, calculationResult, MoonRelativeEarth);

    /* Количество компонент. */
    unsigned componentsCount = calculationResult == CALC_POS ? 3 : 6;

    /* Радиус-вектор (или вектор состояния) Земли относительно барицентра СС. */
    for (unsigned i = 0; i < componentsCount; ++i)
    {
        resultArray[i] -= MoonRelativeEarth[i] * m_emrat2;
    }
}

/* Радиус-вектор (или вектор состояния) Луны относительно барицентра СС. */
void dph::EphemerisRelease::calculateBaseMoon(double JED, unsigned
    calculationResult, double* resultArray) const
{
    /* Радиус-вектор (или вектор состояния) барицентра сиситемы Земля-Луна
       относительно барицентра СС. */
    calculateBaseItem(2, JED, calculationResult, resultArray);

    /* Радиус-вектор (или вектор состояния) Луны относитльно Земли. */
    double MoonRelativeEarth[6];
    calculateBaseItem(9, JED, calculationResult, MoonRelativeEarth);

    /* Количество компонент. */
    unsigned componentsCount = calculationResult == CALC_POS ? 3 : 6;

    /* Относительное положение. */
    for (unsigned i = 0; i < componentsCount; ++i)
    {
        resultArray[i] += MoonRelativeEarth[i] * (1 - m_emrat2);
    }
}




