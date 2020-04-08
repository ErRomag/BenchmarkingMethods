#include "MethodsHAWC.h"
#include "ui_MethodsHAWC.h"

#include <QDebug>
#include <QFile>
#include <QVector>
#include <QMessageBox>
#include <QSharedPointer>
#include "math.h"

MethodsHAWC::MethodsHAWC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MethodsHAWC)
{
    ui->setupUi(this);
    setWindowTitle("Assessment Methods");  // Методы сравнительной оценки технического уровня СТС

    QPixmap pixmapHAMWindow(":/Icon/IconHAMGrad.png");
    QIcon HAMWindowIcon(pixmapHAMWindow);
    setWindowIcon(HAMWindowIcon);

    // Описание MenuBar

    openMainWindowAction = new QAction("&Choose method", this);
    connect(openMainWindowAction, SIGNAL(triggered()), this, SLOT(onOpenMainWindowClicked()));
    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = this->menuBar()->addMenu("&File");

    fileMenu->addAction(openMainWindowAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    NumberLineEdit = 0;

}

MethodsHAWC::~MethodsHAWC()
{
    delete ui;
}

void MethodsHAWC::onOpenMainWindowClicked()
{
    this->close();
    emit openMainWindow();
}

void MethodsHAWC::on_CalculationPushButton_clicked()
{
    fillVectorsWithValuesFromTableWidget();  // Заполняем векторы для расчетов

    // Тут происходит определение метода расчета и сам расчет

    if (ui->HAMRadioButton->isChecked())  // Если МАИ
    {

    }
    else if (ui->WCMRadioButton->isChecked())  // Если МВК
    {
        calculationUsingWeightCoefficientMethod();
    }
}

/// ---------------------  ФУНКЦИИ ДЛЯ МВК  ---------------------

void MethodsHAWC::calculationUsingWeightCoefficientMethod()
{
    QVector<float> kpiFiRowVector;                      // Строка со значениями Кпi
    QVector<QVector<float>> kpiFiTableVector;           // Таблица со строками, хранящимим значения Кпi
    QVector<float> ktuVector;                           // Содержит значния Кту для каждой альтерантивы
    QVector<float> allAverageVector;                    // Хранит сумму средних арифметических для каждой таблицы по каждому показателю
    qint32 HeeSquareValueColumn;                        // Номер столбца в таблице Хи^2
    QVector<QPair<float, QString>> vectorWithKtuAnalyz; // Вектор, хранящий значения Кту и анализ перспективности

    allAverageVector.clear();
    ktuVector.clear();
    vectorWithKtuAnalyz.clear();

    HeeSquareValueColumn =  ui->PforHeeSquareLineEdit->text().toInt();  // Номер столбца для рассчетов Хи^2

    for (qint32 _table = 0; _table < vectorWithVectorValue.size(); _table++)
    {
        qint32 counterColumn = 2;
        float averageCurrentSumm = 0.0;  // Сумма средних арифметических для текущей таблицы
        QVector<float> averageForCurrentTable;  // Хранит средние арифметические для текущей таблицы

        averageForCurrentTable = checkOpinionAndGetAverageVector(vectorWithVectorExperts.at(_table), HeeSquareValueColumn, _table);
        averageCurrentSumm = getVectorSumm(averageForCurrentTable);
        allAverageVector.push_back(averageCurrentSumm);  // Запоминаем сумму средних арифметических для рассчетов

        while (counterColumn < vectorWithVectorValue.at(_table).at(0).size()) // Идем по столбцам
        {
            float kpi = 0.0;  // Значение Кпi для текущего образца

            for (qint32 _row = 0; _row < vectorWithVectorValue.at(_table).size(); _row++)  // Идем по строкам в столбце
            {

                if (vectorWithVectorValue.at(_table).at(_row).at(0) == 1)
                {
                    kpi += (vectorWithVectorValue.at(_table).at(_row).at(counterColumn) / vectorWithVectorValue.at(_table).at(_row).at(1) )
                            * averageForCurrentTable.at(_row);

                } else if (vectorWithVectorValue.at(_table).at(_row).at(0) == -1)
                {
                    kpi += (vectorWithVectorValue.at(_table).at(_row).at(1) / vectorWithVectorValue.at(_table).at(_row).at(counterColumn) )
                            * averageForCurrentTable.at(_row);

                } else
                {
                    // Вывод сообщения о том, что неправильно указано направление влияния ТУ
                    QMessageBox::warning(this, "Error", "In table " + QString::number(_table) +
                                         "\n incorrect direction of influence " +
                                         "\n of the technical level coefficient.");
                }
            }
            kpiFiRowVector.push_back(kpi);
            counterColumn++;
        }
        kpiFiTableVector.push_back(kpiFiRowVector);
        kpiFiRowVector.clear();
    }

    qint32 currentColumn = 0;
    float resultAverageSumm = getVectorSumm(allAverageVector);  // Общая сумма средних арифметических по всем таблицам

    // Находим сумму по всем альтенитивам по каждой таблице
    while (currentColumn < kpiFiTableVector.at(0).size())
    {
        float currentKtu = 0.0;  // Текущее значение Kту
        float currentKpiFiSumm = 0.0;

        for (qint32 _row = 0; _row < kpiFiTableVector.size(); _row++)
        {
            currentKpiFiSumm += kpiFiTableVector.at(_row).at(currentColumn);
        }
        currentKtu = currentKpiFiSumm / resultAverageSumm;  // Делим на сумму средних арифметически
        ktuVector.push_back(currentKtu);
        currentColumn++;
    }

    // Формируем пары значение Кту и вывод о его перспективности
    for (qint32 i = 0; i < ktuVector.size(); i++)
    {
        QPair<float, QString> ktuWithAnalys;
        ktuWithAnalys.first = ktuVector.at(i);
        ktuWithAnalys.second = assessProspects(ktuVector.at(i));
        vectorWithKtuAnalyz.push_back(ktuWithAnalys);
    }

    // Найти среди них максимальный элемент и покрасить его в зеленый или как нибудь еще обозначить

    float maxNumberInKtuVector = ktuVector.at(0);

    for (qint32 i = 0; i < ktuVector.size(); i++)
    {
        if (ktuVector.at(i) > maxNumberInKtuVector)
        {
            maxNumberInKtuVector = ktuVector.at(i);
        }
    }

    // Вывод в таблицу итоговую на форме

    ui->finalPrioritetsTableWidget->setRowCount(NumberAlternative - 1);
    ui->finalPrioritetsTableWidget->setColumnCount(3);
    ui->finalPrioritetsTableWidget->setHorizontalHeaderLabels(QStringList() << "#" << "Ktu" << "Prospective");

    for (qint32 _row = 0; _row < vectorWithKtuAnalyz.size(); _row++)
    {
        QTableWidgetItem *numberKtu = new QTableWidgetItem(QString::number(_row + 3));
        ui->finalPrioritetsTableWidget->setItem(_row, 0, numberKtu);
        ui->finalPrioritetsTableWidget->setColumnWidth(0, 30);

        QTableWidgetItem *ktuValue = new QTableWidgetItem(QString::number(vectorWithKtuAnalyz.at(_row).first));
        ui->finalPrioritetsTableWidget->setItem(_row, 1, ktuValue);
        ui->finalPrioritetsTableWidget->setColumnWidth(1, 60);

        QTableWidgetItem *assessKtu = new QTableWidgetItem(vectorWithKtuAnalyz.at(_row).second);
        ui->finalPrioritetsTableWidget->setItem(_row, 2, assessKtu);
        ui->finalPrioritetsTableWidget->setColumnWidth(2, 120);
    }

    qDebug() << "vectorWithKtuAnalyz = " << vectorWithKtuAnalyz;
    qDebug() << "maxNumberInKtuVector = " << maxNumberInKtuVector;
}

QVector<float> MethodsHAWC::checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber, qint32 _tableNumber)
{
    qint32 numberIndex;             // Кол-во показателей
    qint32 numberExperts;           // Кол-во экспертов
    QVector <float> averageVector;  // Выходной вектор со средними арифметическими
    QVector<float> VVector;         // среднее арифметическое
    QVector<float> SCOVector;       // Вектор СКО

    numberIndex = _vectorFromTableWidget.size();
    numberExperts = _vectorFromTableWidget.at(0).size();

    // РАСЧЕТ КОЭФФИЦИЕНТА КОНКОРДАЦИИ И КОЭФФИЦИЕНТА ХИ^2
    float omega = 0.0;                        // Коэффициент конкордации
    float actualValueHeeSquare = 0.0;         // Фактическое значение Хи^2
    float theoreticalValueHeeSquare = getHeeSquareTableValue(numberIndex, _columnNumber);  // Брем из таблицы

    averageVector.clear();
    SCOVector.clear();
    VVector.clear();

    /* Если количество показателей = 1, то в формуле Хи^2 будет деление на 0,
     * что недопустимо, поэтому считаем, что коэффициенты равны 0
     */
    if (numberIndex == 0 | numberExperts == 0)
    {
        QMessageBox::warning(this, "Warning!", "Количество экспертов или показателей равно 0!");
    }
    else if ( numberIndex == 1)
    {
        omega = 0;
        actualValueHeeSquare = 0;

    } else {

        // Вычисление средного арифметического оценок экспертов для каждого показателя
        float PsiI = 0.0;
        for (int i = 0; i < numberIndex; i++)
        {
            PsiI = 0.0;
            for(int j = 0; j < numberExperts; j++)
            {
                PsiI += _vectorFromTableWidget.at(i).at(j);
            }
            PsiI /= _vectorFromTableWidget.at(i).size();
            averageVector.push_back(PsiI);
        }

        // Вычисление СКО для каждого показателя
        float SCO = 0.0;
        float SCOsum = 0.0;

        for (int i = 0; i < numberIndex; i++)
        {
            SCO = 0.0;
            for(int j = 0; j < numberExperts; j++)
            {
                SCO = _vectorFromTableWidget.at(i).at(j) - averageVector.at(i);
                SCO = SCO * SCO;
                SCOsum += SCO;
            }
            SCOsum /= _vectorFromTableWidget.at(i).size();
            SCOsum = sqrt(SCOsum);
            SCOVector.push_back(SCOsum);
        }

        // Высисление коэффициента изменчисвости для каждого показателя
        float V = 0.0;  // Коэффициент изменчивости

        for (int i = 0; i < numberIndex; i++)
        {
            V = 0.0;
            V = SCOVector.at(i) / averageVector.at(i);
            VVector.push_back(V);
        }

        QVector< QVector<qint32> > SearchNumberVector;  // Вектор, хранящий количество повторений каждого числа
        QVector<qint32> currentVector;
        QVector<int> eccourededNumber;       // Количество втречающихся элементов в строке
        qint32 currentEccourededNumber = 0;  // Количество текущих повторений
        int currentNumberSearch = 10;        // текущий номер, который ищем

        SearchNumberVector.clear();
        currentVector.clear();
        eccourededNumber.clear();

        for (int i = 0; i < numberIndex; i++)
        {
            currentNumberSearch = 10;
            eccourededNumber.clear();

            while (currentNumberSearch >= 1)  // Перебираем все числа от 1 до 10 из оценок экспертов
            {
                currentEccourededNumber = 0;

                for(int j = 0; j < numberExperts; j++)
                {
                    // Находим количество повторений для каждого числа
                    if (_vectorFromTableWidget.at(i).at(j) == currentNumberSearch) currentEccourededNumber++;

                }
                eccourededNumber.push_back(currentEccourededNumber);
                currentNumberSearch--;
            }
            SearchNumberVector.push_back(eccourededNumber);
        }

        // Считаем l1 ... ln, находим их сумму, чтобы найти S

        QVector<qint32> realNumberPool;  /* Пул, в котором хранятся целые числа для вычислений,
                                          * для количетсва экспертов = 9 пул будет: {1,2,3,4,5,6,7,8,9}
                                          */
        QVector<float> vectorS;                      // Среднее арифметическое сумм рангов
        float currentLi = 0.0;                       // Текущее среднее арифметическое сумм рангов
        float currentNumberFromSearchVector = 0;     // Текущее количество повторений
        float summFromRealNumberPool = 0;            // Текущая сумма из вектора целых чисел
        qint32 currentPositionInRealNumberPool = 0;  // Текущая позиция счетчика в пуле целых чисел
        float summS = 0.0;                           // Сумма соответсвующего столбца li
        float S = 0.0;                               // Сумма

        vectorS.clear();
        realNumberPool.clear();

        for (int i = 0; i < numberExperts; i++)  // Заполнение пула
        {
            realNumberPool.push_back(i + 1);
        }

        /* Тут происходит магия расчетов li. Для вычислений берется количество повторений каждого
         * мнения эксперта (это знаменатель), в числителе - сумма натуральных чисел (от 1 до кол-ва
         * экспертов), по количеству равная повторениям, то есть если повторений было 3, то в числителе
         * будет стоять: 1 + 2 + 3.
         */

        for (int i = 0; i <  numberIndex; i++)
        {
            summS = 0.0;
            currentPositionInRealNumberPool = 0;

            for (int j = 0; j < SearchNumberVector.at(i).size(); j++)
            {
                currentNumberFromSearchVector = SearchNumberVector.at(i).at(j);  // Берем количество повторений
                summFromRealNumberPool = 0.0;

                for (int k = currentPositionInRealNumberPool;
                     k < currentNumberFromSearchVector + currentPositionInRealNumberPool; k++)
                {
                    summFromRealNumberPool += realNumberPool.at(k);
                }

                currentPositionInRealNumberPool += currentNumberFromSearchVector;

                if (currentNumberFromSearchVector == 0)
                {
                    currentLi = 0;

                } else {
                    currentLi = summFromRealNumberPool / currentNumberFromSearchVector;
                }

                summS += currentLi;
            }

            S += summS;
            vectorS.push_back(summS);
        }
        S /= numberIndex;


        float summDiSqrt = 0.0;  // Сумма отклонений в квадрате для коэфф. конкордации и значения критерия Пирсона

        for (int i = 0; i < vectorS.size(); i++)
        {
            summDiSqrt += pow((vectorS.at(i) - S), 2);
        }

        float summTi = 0.0;  /* Показатель совокупности рангов по i-му показателю
                              * (сразу рассчитывается сумма, так как только она понадобится в дальнейшем)
                              */

        QVector<float> vectorTi;  // Нужен, чтобы вывести в таблицу значения, если необходимо будет

        for (int i = 0; i < SearchNumberVector.size(); i++)
        {
            for (int j = 0; j < SearchNumberVector.at(i).size(); j++)
            {
                summTi += (pow(SearchNumberVector.at(i).at(j), 3) - SearchNumberVector.at(i).at(j));
            }
        }

        omega = (12 * summDiSqrt) / (pow(numberExperts, 2) * (pow(numberIndex, 3) - 1) - numberExperts * summTi);
        actualValueHeeSquare = (12 * summDiSqrt) / (numberExperts * numberIndex * (numberIndex + 1) - (1 / (numberIndex - 1)) * summTi);
    }

    if (omega > 0 && (actualValueHeeSquare >= theoreticalValueHeeSquare ))
    {
        // Если мнения согласованы
        QMessageBox::information(this, "Information", "In table " + QString::number(_tableNumber + 1)
                                 + " expert opinions agreed.");
        return  averageVector;

    } else {

        // Анализ мнений экспертов, когда они не согласованы
        float averageSummVVector = 0.0;  // Среднее арифметическое коэффициентов изменчивости
        QString stringWithPosition;      /* Строка с позициями, чтобы нормально вывести тут хранятся
                                          * позиции элементов, которые оказались
                                          * больше среднего арифметического и их надо скорректировать
                                          */

        stringWithPosition = "";
        averageSummVVector = getVectorSumm(VVector);
        averageSummVVector /= numberIndex;

        for (qint32 i = 0; i < VVector.size(); i++)  // Находим показатели, которые надо скорректировать
        {
            if (VVector.at(i) >= averageSummVVector) stringWithPosition += QString::number(i + 1) + " ";
        }

        QMessageBox::warning(this, "Error", "Expert opinions are not agreed. \n Please check index " + stringWithPosition
                             + "\n in table " + QString::number(_tableNumber + 1));
        VVector.push_back(-42);  // Флаг, показывающий, что мнения не согласованы и дальнейшие расчеты не имеют смысла
        return averageVector;
    }
}

// Вспомогательная функция, сравнивающая два числа с плавающей точкой
bool MethodsHAWC::equalFloatingValue(float _value1, float _value2, float _epsilon)
{
    if ( abs(_value1 - _value2) < _epsilon )
    {
        return true;
    } else
    {
        return false;
    }
}

float MethodsHAWC::getHeeSquareTableValue(int _numberIndex, int _nu)
{
    // Таблица значений Хи^2
    float heeSquareTableMatrixNew [30][13]  = {
        // 0.99   0.98   0.95   0.90   0.80
        /*1(0)*/    { 0.000, 0.001, 0.004, 0.016, 0.064 },
        /*2(1)*/    { 0.020, 0.040, 0.103, 0.211, 0.446 },
        /*3(2)*/    { 0.115, 0.185, 0.352, 0.584, 1.005 },
        /*4(3)*/    { 0.297, 0.429, 0.711, 1.064, 1.649 },
        /*5(4)*/    { 0.554, 0.752, 1.145, 1.610, 2.340 },
        /*6(5)*/    { 0.872, 1.134, 1.635, 2.200, 3.070 },

        /*7(6)*/    { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*8(7)*/    { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*9(8)*/    { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*10(9)*/   { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*11(10)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*12(11)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*13(12)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*14(13)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*15(14)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*16(15)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*17(16)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*18(17)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*19(18)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*20(19)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*21(20)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*22(21)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*23(22)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*24(23)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*25(24)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*26(25)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*27(26)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*28(27)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*29(28)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
        /*30(29)*/  { 0.872, 1.134, 1.635, 2.200, 3.070 },
    };

    qint32 r;       // Номер строки в таблице
    float p;        // Вероятность, что величина распределения Хи^2 с заданным r не превзойдет это значение
    qint32 _column; // Номер столбца в таблице

    r = _numberIndex - 1 - 1; // Первый "-1" по формуле, второй "-1", потому что считаем от 0
    p = 1 - _nu;

    if (equalFloatingValue(0.99, p, 0.01)) {
        _column = 0;
    }
    else if (equalFloatingValue(0.98, p, 0.01)) {
        _column = 1;
    }
    else if (equalFloatingValue(0.95, p, 0.01)) {
        _column = 2;
    }
    else if (equalFloatingValue(0.90, p, 0.01)) {
        _column = 3;
    }
    else if (equalFloatingValue(0.80, p, 0.01)) {
        _column = 4;
    }
    else if (equalFloatingValue(0.70, p, 0.01)) {
        _column = 5;
    }
    else if (equalFloatingValue(0.50, p, 0.01)) {
        _column = 6;
    }
    else if (equalFloatingValue(0.30, p, 0.01)) {
        _column = 7;
    }
    else if (equalFloatingValue(0.20, p, 0.01)) {
        _column = 8;
    }
    else if (equalFloatingValue(0.10, p, 0.01)) {
        _column = 9;
    }
    else if (equalFloatingValue(0.05, p, 0.01)) {
        _column = 10;
    }
    else if (equalFloatingValue(0.02, p, 0.01)) {
        _column = 11;
    }
    else if (equalFloatingValue(0.01, p, 0.01)) {
        _column = 12;
    }
    else if (equalFloatingValue(0.001, p, 0.001)) {
        _column = 13;
    }
    else {
        QMessageBox::warning(this, "Error", "Value nu is invalid");
    }

    return heeSquareTableMatrixNew[r][_column];
}

void MethodsHAWC::fillVectorsWithValuesFromTableWidget()
{
    vectorWithVectorValue.clear();
    vectorWithVectorExperts.clear();

    for (qint32 i = 0; i < tableWidgetValueVector.size(); i++)
    {
        vectorWithVectorValue.push_back(getQVectorFromQTableWidget(tableWidgetValueVector.at(i)));
    }

    for (qint32 i = 0; i < tableWidgetExpertVector.size(); i++)
    {
        vectorWithVectorExperts.push_back(getQVectorFromQTableWidget(tableWidgetExpertVector.at(i)));
    }
}

float MethodsHAWC::getVectorSumm(QVector<float> _dataVector)
{
    float vectorSumm = 0.0;

    for (qint32 i = 0; i < _dataVector.size(); i++)
    {
        vectorSumm += _dataVector.at(i);
    }

    return  vectorSumm;
}

QString MethodsHAWC::assessProspects(float _ktu)
{
    QString prospectsResult;

    if (_ktu < 1.065 )                 return prospectsResult = "Not promising";
    if (_ktu > 1.065  && _ktu < 1.13 ) return prospectsResult = "Unpromising";
    if (_ktu > 1.13   && _ktu < 1.27 ) return prospectsResult = "Promising";
    if (_ktu > 1.27  )                 return prospectsResult = "Very promising";
}

/// ---------------------  ОБЩИЕ ФУНКЦИИ  -------------------------

/* При нажатии на кнопку в Layout создаютя LineEdit
 * для ввода количества единичных показателей,
 * соответствующие каждому интегральному показателю
 */
void MethodsHAWC::on_CreateSingleIndexPushButton_clicked()
{
    NumberIntegralIndex = ui->NumberIntegralIndexLineEdit->text().toInt();

    for (qint32 i = 0; i < NumberIntegralIndex; i++)
    {
        // QSharedPointer<QLineEdit> lineEdit(new QLineEdit());
        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setPlaceholderText(QString::number(NumberLineEdit + 1) + " Integral Index");
        ui->IntegralIndexVerticalLayout->addWidget(lineEdit);
        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // Сигнал от LineEdit вызывается, когда с него снят фокус
        NumberLineEdit++;
    }
}

// СЛОТ для получения вектора, в котором хранится количество единичных показателей для каждого интегрального
void MethodsHAWC::slotGetNumberUnitsIndex()
{
    QLineEdit *lineEdit = (QLineEdit*) sender();
    NumberUnitsIndexVector.push_back(lineEdit->text().toInt());
}

void MethodsHAWC::on_CreateTablesPushButton_clicked()
{
    NumberExperts = ui->NumberExpertsLineEdit->text().toInt();
    NumberAlternative = ui->NumberAlternativeLineEdit->text().toInt();

    tableWidgetValueVector.clear();
    tableWidgetExpertVector.clear();

    if (ui->HAMRadioButton->isChecked())
    {
        // Создаение таблицы для мнений экспертов по ИП
        QTableWidget *integralIndexTableWidgetExperts = new QTableWidget();
        integralIndexTableWidgetExperts->setRowCount(NumberIntegralIndex);
        integralIndexTableWidgetExperts->setColumnCount(NumberExperts);
        resizeColumnWidth(integralIndexTableWidgetExperts);
        ui->ExpertsVerticalLayout->addWidget(integralIndexTableWidgetExperts);
        tableWidgetExpertVector.push_back(integralIndexTableWidgetExperts);
    }

    for (qint32 i = 0; i < NumberIntegralIndex; i++)
    {
        // Создание таблиц числовых значений
        QTableWidget *tableWidgetValue = new QTableWidget();
        tableWidgetValue->setRowCount(NumberUnitsIndexVector.at(i));

        // Для МВК +1 столбец нужен для записи направления роста ТУ (1 - повышение, -1 - понижение)
        if (ui->WCMRadioButton->isChecked())
        {
            tableWidgetValue->setColumnCount(NumberAlternative + 1);
        } else {
            tableWidgetValue->setColumnCount(NumberAlternative);
        }
        resizeColumnWidth(tableWidgetValue);
        //tableWidgetValue->resize(100, 100);
        ui->layout->addWidget(tableWidgetValue);
        tableWidgetValueVector.push_back(tableWidgetValue);

        // Создание таблиц для оценок экспертов
        QTableWidget *tableWidgetExperts = new QTableWidget();
        tableWidgetExperts->setRowCount(NumberUnitsIndexVector.at(i));
        tableWidgetExperts->setColumnCount(NumberExperts);
        resizeColumnWidth(tableWidgetExperts);
        //tableWidgetExperts->resize(100, 100);
        ui->ExpertsVerticalLayout->addWidget(tableWidgetExperts);
        tableWidgetExpertVector.push_back(tableWidgetExperts);
    }

    //    qDebug() << tableWidgetValueVector;
    //    qDebug() << tableWidgetExpertVector;
}

QVector<QVector<float>> MethodsHAWC::getQVectorFromQTableWidget(QTableWidget *_tableWidget)
{
    qint32 numberIndex;     // Кол-во строк
    qint32 numberExperts;  // Кол-во столбцов
    QVector<QVector<float> > vectorFromTableWidget;  // Выходной вектор
    QVector <float> vectorRows;  // Вектор, хранящий строку

    numberIndex = _tableWidget->rowCount();
    numberExperts = _tableWidget->columnCount();

    for (qint32 i = 0; i < numberIndex; i++)
    {
        for (qint32 j = 0; j < numberExperts; j++)
        {
            if (_tableWidget->item(i, j)->text() == "")
            {
                QMessageBox::warning(this, "Error", "Not all values entered in tables. \n Check tables.");
                break;
            } else {
                vectorRows.push_back(_tableWidget->item(i, j)->text().toFloat());
            }
        }
        vectorFromTableWidget.push_back(vectorRows);
        vectorRows.clear();
    }

    return  vectorFromTableWidget;
}

void MethodsHAWC::resizeColumnWidth(QTableWidget *_tableWidget)
{
    for (qint32 _column = 0; _column < _tableWidget->columnCount(); _column++)
    {
        _tableWidget->setColumnWidth(_column, 60);
    }
}

void MethodsHAWC::saveAsCSV(QString filename, QTableWidget _tableWidget)  // Сохранение таблицы в .SCV формат
{
    QFile tableFile(filename);

    if( tableFile.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &tableFile );
        QStringList strList;

        strList << "\" \"";
        for( int c = 0; c < _tableWidget.horizontalHeader()->count(); ++c )
            strList << "\"" + _tableWidget.model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < _tableWidget.verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\"" + _tableWidget.model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < _tableWidget.horizontalHeader()->count(); ++c )
            {
                strList << "\"" + _tableWidget.model()->data(_tableWidget.model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }

        tableFile.close();
    }
}

void MethodsHAWC::on_TEST_BTN_clicked()
{
    QVector<QVector<float>> testVector;
    QVector<float> testAverageVector;

    testVector = getQVectorFromQTableWidget(ui->tableWidget);

    qDebug() << testVector;
    testAverageVector =  checkOpinionAndGetAverageVector(testVector, 3, 2);
}


