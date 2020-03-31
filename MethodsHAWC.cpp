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


    ui->tableWidget->setRowCount(3);  // Альтенативы
    ui->tableWidget->setColumnCount(5);  // Эксеперты
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "a" << "b" << "c" << "d" << "e");

    // int count = 1;
    //    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    //    {
    //        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
    //        {
    //            QTableWidgetItem *item = new QTableWidgetItem(QString::number(count*count));
    //            ui->tableWidget->setItem(i, j, item);
    //            count++;
    //        }
    //    }

    //    ui->scrollArea->setWidgetResizable(false);
    //    ui->scrollArea->setLayout(ui->layout);

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
        HeeSquareValueColumn =  ui->PforHeeSquareLineEdit->text().toInt();  // Номер столбца для рассчетов Хи^2

        allAverageVector.clear();

        for (qint32 _table = 0; _table < vectorWithVectorExperts.size(); _table++)
        {
            qint32 counterColumn = 2;
            float averageCurrentSumm = 0.0;  // Сумма средних арифметических для текущей таблицы
            QVector<float> averageForCurrentTable;  // Хранит средние арифметические для текущей таблицы

            averageForCurrentTable = checkOpinionAndGetAverageVector(vectorWithVectorExperts.at(_table), HeeSquareValueColumn, _table);
            averageCurrentSumm = getVectorSumm(averageForCurrentTable);
            allAverageVector.push_back(averageCurrentSumm);  // Запоминаем сумму средних арифметических для рассчетов

            while (counterColumn <= vectorWithVectorExperts.at(_table).at(0).size()) // Идем по столбцам
            {
                float kpi = 0.0;  // Значение Кпi для текущего образца

                for (qint32 _row = 0; _row < vectorWithVectorExperts.at(_table).size(); _row++)  // Идем по строкам в столбце
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

        qDebug() << "kpiFiTableVector = " << kpiFiTableVector;
        qDebug() <<"allAverageVector = " << allAverageVector;

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
            currentKtu = currentKpiFiSumm / resultAverageSumm;
            ktuVector.push_back(currentKtu);
            currentColumn++;
        }

        qDebug() << "ktuVector = " << ktuVector;
    }
}

/* При нажатии на кнопку в Layout создаютя LineEdit для ввода количества единичных показателей,
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


    //    ui->IntegralIndexVerticalLayout->addWidget(lineEdit);  // Добавляем кнопку в слой с вертикальной компоновкой
    //    lineEdit->setPlaceholderText(QString::number(NumberLineEdit + 1) + " Integral Index");
    //    NumberLineEdit++;
    //    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // Сигнал от LineEdit вызывается, когда с него снят фокус

    //ui->scrollArea->setWidgetResizable(true);

    //    for (int k = 0; k < NumberIntegralIndex; k++)
    //    {
    //        // DynamicLineEdit *lineEdit = new DynamicLineEdit();  // Создаем объект динамической кнопки
    //        QLineEdit *lineEdit = new QLineEdit();
    //        QTableWidget *tableWidget = new QTableWidget;
    //        QLabel *label = new QLabel;
    //        QSharedPointer<QTableWidget> tableWidget1(new QTableWidget);


    //        label->setText("Integral Index " + QString::number(NumberLineEdit));
    //        tableWidget->setRowCount(6);
    //        tableWidget->setColumnCount(5);
    //        tableWidget->setHorizontalHeaderLabels(QStringList() << "a" << "b" << "c" << "d" << "e");
    //        //tableWidget->setMinimumWidth(100);
    //        //tableWidget->setMinimumHeight(100);
    //        //ui->layout->addWidget(label);
    //        //ui->layout->addWidget(tableWidget);
    //        ui->layout->addWidget(tableWidget);

    //        //tableWidget->resize(2000, 2000);

    //        //        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //        ui->scrollArea->setWidgetResizable(true);
    //        //ui->scrollArea->setLayout(layout);

    //        int count = 1;
    //        for (int i = 0; i <tableWidget->rowCount(); i++)
    //        {
    //            for (int j = 0; j < tableWidget->columnCount(); j++)
    //            {
    //                QTableWidgetItem *item = new QTableWidgetItem(QString::number(count*count));
    //                tableWidget->setItem(i, j, item);
    //                count++;
    //            }
    //        }

    //        ui->IntegralIndexVerticalLayout->addWidget(lineEdit);  // Добавляем кнопку в слой с вертикальной компоновкой
    //        lineEdit->setPlaceholderText(QString::number(NumberLineEdit + 1) + " Integral Index");
    //        NumberLineEdit++;
    //        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // Сигнал от LineEdit вызывается, когда с него снят фокус
    //    }
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
        ui->ExpertsVerticalLayout->addWidget(integralIndexTableWidgetExperts);
        tableWidgetExpertVector.push_back(integralIndexTableWidgetExperts);
    }

    for (qint32 i = 0; i < NumberIntegralIndex; i++)
    {
        // Создание таблиц числовых значений
        QTableWidget *tableWidgetValue = new QTableWidget();
        tableWidgetValue->setRowCount(NumberUnitsIndexVector.at(i));
        tableWidgetValue->setColumnCount(NumberAlternative + 1);  // + 1 столбец нужен для записи направления роста ТУ (1 - повышение, 0 - понижение)
        tableWidgetValue->resize(100, 100);
        ui->layout->addWidget(tableWidgetValue);
        tableWidgetValueVector.push_back(tableWidgetValue);

        // Создание таблиц для оценок экспертов
        QTableWidget *tableWidgetExperts = new QTableWidget();
        tableWidgetExperts->setRowCount(NumberUnitsIndexVector.at(i));
        tableWidgetExperts->setColumnCount(NumberExperts);
        tableWidgetExperts->resize(100, 100);
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
            vectorRows.push_back(_tableWidget->item(i, j)->text().toFloat());
        }
        vectorFromTableWidget.push_back(vectorRows);
        vectorRows.clear();
    }

    return  vectorFromTableWidget;
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

        for (qint32 i = 0; i < VVector.size(); i++)
        {
            averageSummVVector += VVector.at(i);
        }
        averageSummVVector /= numberIndex;

        for (qint32 i = 0; i < VVector.size(); i++)  // Находим показатели, которые надо скорректировать
        {
            if (VVector.at(i) >= averageSummVVector) stringWithPosition += QString::number(i + 1) + " ";
        }

        QMessageBox::information(this, "Error", "Expert opinions are not agreed. \n Please check index " + stringWithPosition
                                 + "\n in table " + QString::number(_tableNumber + 1));
        VVector.push_back(-42);
        // VVector[0] = -42;  // Флаг, показывающий, что мнения не согласованы и дальнейшие расчеты не имеют смысла
        return VVector;
    }
}

float MethodsHAWC::getHeeSquareTableValue(int n, int p)
{
    int   r   = n - 1 - 1;  // Первый "-1" по формуле, второй "-1", потому что считаем от 0
    float valueInRP;

    valueInRP = heeSquareTableMatrixNew[r][p];

    return valueInRP;
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


