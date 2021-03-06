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
    setWindowTitle("Assessment Methods");  // ������ ������������� ������ ������������ ������ ���

    QPixmap pixmapHAMWindow(":/Icon/IconHAMGrad.png");
    QIcon HAMWindowIcon(pixmapHAMWindow);
    setWindowIcon(HAMWindowIcon);

    // MenuBar
    openMainWindowAction = new QAction("&Main menu", this);
    connect(openMainWindowAction, SIGNAL(triggered()), this, SLOT(onOpenMainWindowClicked()));
    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = this->menuBar()->addMenu("&File");

    fileMenu->addAction(openMainWindowAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    NumberLineEdit = 0;
    ui->WCMRadioButton->setChecked(true);
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
    fillVectorsWithValuesFromTableWidget();  // ��������� ������� ��� ��������

    if (ui->HAMRadioButton->isChecked())  // ���� ���
    {
        calculationUsingHierarchyAnalysisMethod();
    }
    else if (ui->WCMRadioButton->isChecked())  // ���� ���
    {
        if (ui->PforHeeSquareLineEdit->text() == "")
        {
            QMessageBox::warning(this, "Error", "Nu value not entered");
        } else
        {
            calculationUsingWeightCoefficientMethod();
        }
    }
}

/// ---------------------  ������� ��� ���  ---------------------

void MethodsHAWC::calculationUsingWeightCoefficientMethod()
{
    QVector<float> kpiFiRowVector;                      // ������ �� ���������� ��i
    QVector<QVector<float>> kpiFiTableVector;           // ������� �� ��������, ���������� �������� ��i
    QVector<float> ktuVector;                           // �������� ������� ��� ��� ������ ������������
    QVector<float> allAverageVector;                    // ������ ����� ������� �������������� ��� ������ ������� �� ������� ����������
    float HeeSquareValueColumn;                        // ����� ������� � ������� ��^2
    QVector<QPair<float, QString>> vectorWithKtuAnalyz; // ������, �������� �������� ��� � ������ ���������������

    allAverageVector.clear();
    ktuVector.clear();
    vectorWithKtuAnalyz.clear();

    HeeSquareValueColumn =  ui->PforHeeSquareLineEdit->text().toFloat();  // ����� ������� ��� ��������� ��^2

    for (qint32 _table = 0; _table < vectorWithVectorValue.size(); _table++)
    {
        qint32 counterColumn = 2;
        float averageCurrentSumm = 0.0;  // ����� ������� �������������� ��� ������� �������
        QVector<float> averageForCurrentTable;  // ������ ������� �������������� ��� ������� �������

        averageForCurrentTable = checkOpinionAndGetAverageVector(vectorWithVectorExperts.at(_table), HeeSquareValueColumn, _table);
        averageCurrentSumm = getVectorSumm(averageForCurrentTable);
        allAverageVector.push_back(averageCurrentSumm);  // ���������� ����� ������� �������������� ��� ���������

        while (counterColumn < vectorWithVectorValue.at(_table).at(0).size()) // ���� �� ��������
        {
            float kpi = 0.0;  // �������� ��i ��� �������� �������

            for (qint32 _row = 0; _row < vectorWithVectorValue.at(_table).size(); _row++)  // ���� �� ������� � �������
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
                    // ����� ��������� � ���, ��� ����������� ������� ����������� ������� ��
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
    float resultAverageSumm = getVectorSumm(allAverageVector);  // ����� ����� ������� �������������� �� ���� ��������

    // ������� ����� �� ���� ������������ �� ������ �������
    while (currentColumn < kpiFiTableVector.at(0).size())
    {
        float currentKtu = 0.0;  // ������� �������� K��
        float currentKpiFiSumm = 0.0;

        for (qint32 _row = 0; _row < kpiFiTableVector.size(); _row++)
        {
            currentKpiFiSumm += kpiFiTableVector.at(_row).at(currentColumn);
        }
        currentKtu = currentKpiFiSumm / resultAverageSumm;  // ����� �� ����� ������� �������������
        ktuVector.push_back(currentKtu);
        currentColumn++;
    }

    // ��������� ���� �������� ��� � ����� � ��� ���������������
    for (qint32 i = 0; i < ktuVector.size(); i++)
    {
        QPair<float, QString> ktuWithAnalys;
        ktuWithAnalys.first = ktuVector.at(i);
        ktuWithAnalys.second = assessProspects(ktuVector.at(i));
        vectorWithKtuAnalyz.push_back(ktuWithAnalys);
    }

    // ����� ����� ��� ������������ ������� � ��������� ��� � ������� ��� ��� ������ ��� ����������

    float maxNumberInKtuVector = ktuVector.at(0);

    for (qint32 i = 0; i < ktuVector.size(); i++)
    {
        if (ktuVector.at(i) > maxNumberInKtuVector)
        {
            maxNumberInKtuVector = ktuVector.at(i);
        }
    }

    // ����� � ������� �������� �� �����

    ui->finalPrioritetsTableWidget->setRowCount(NumberAlternative - 1);
    ui->finalPrioritetsTableWidget->setColumnCount(3);
    ui->finalPrioritetsTableWidget->setHorizontalHeaderLabels(QStringList() << "#" << "Ktu" << "Promising");

    for (qint32 _row = 0; _row < vectorWithKtuAnalyz.size(); _row++)
    {
        QTableWidgetItem *numberKtu = new QTableWidgetItem(QString::number(_row + 1));
        ui->finalPrioritetsTableWidget->setItem(_row, 0, numberKtu);
        ui->finalPrioritetsTableWidget->setColumnWidth(0, 30);

        QTableWidgetItem *ktuValue = new QTableWidgetItem(QString::number(vectorWithKtuAnalyz.at(_row).first));
        ui->finalPrioritetsTableWidget->setItem(_row, 1, ktuValue);
        ui->finalPrioritetsTableWidget->setColumnWidth(1, 60);

        QTableWidgetItem *assessKtu = new QTableWidgetItem(vectorWithKtuAnalyz.at(_row).second);
        ui->finalPrioritetsTableWidget->setItem(_row, 2, assessKtu);
        ui->finalPrioritetsTableWidget->setColumnWidth(2, 120);
    }
}

QVector<float> MethodsHAWC::checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, float _heeSquareCoefficient, qint32 _tableNumber)
{
    qint32 numberIndex;             // ���-�� �����������
    qint32 numberExperts;           // ���-�� ���������
    QVector <float> averageVector;  // �������� ������ �� �������� ���������������
    QVector<float> VVector;         // ������� ��������������
    QVector<float> SCOVector;       // ������ ���

    numberIndex = _vectorFromTableWidget.size();
    numberExperts = _vectorFromTableWidget.at(0).size();

    // ������ ������������ ����������� � ������������ ��^2
    float omega = 0.0;                        // ����������� �����������
    float actualValueHeeSquare = 0.0;         // ����������� �������� ��^2
    float theoreticalValueHeeSquare = getHeeSquareTableValue(numberIndex, _heeSquareCoefficient);  // ���� �� �������

    averageVector.clear();
    SCOVector.clear();
    VVector.clear();

    /* ���� ���������� ����������� = 1, �� � ������� ��^2 ����� ������� �� 0,
     * ��� �����������, ������� �������, ��� ������������ ����� 0
     */
    if (numberIndex == 0 | numberExperts == 0)
    {
        QMessageBox::warning(this, "Warning!", "The number of experts or inex is 0!");
    } else {

        // ���������� �������� ��������������� ������ ��������� ��� ������� ����������
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

        // ���������� ��� ��� ������� ����������
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

        // ���������� ������������ ������������� ��� ������� ����������
        float V = 0.0;  // ����������� ������������

        for (int i = 0; i < numberIndex; i++)
        {
            V = 0.0;
            V = SCOVector.at(i) / averageVector.at(i);
            VVector.push_back(V);
        }

        QVector< QVector<qint32> > SearchNumberVector;  // ������, �������� ���������� ���������� ������� �����
        QVector<qint32> currentVector;
        QVector<int> eccourededNumber;       // ���������� ������������ ��������� � ������
        qint32 currentEccourededNumber = 0;  // ���������� ������� ����������
        int currentNumberSearch = 10;        // ������� �����, ������� ����

        SearchNumberVector.clear();
        currentVector.clear();
        eccourededNumber.clear();

        for (int i = 0; i < numberIndex; i++)
        {
            currentNumberSearch = 10;
            eccourededNumber.clear();

            while (currentNumberSearch >= 1)  // ���������� ��� ����� �� 1 �� 10 �� ������ ���������
            {
                currentEccourededNumber = 0;

                for(int j = 0; j < numberExperts; j++)
                {
                    // ������� ���������� ���������� ��� ������� �����
                    if (_vectorFromTableWidget.at(i).at(j) == currentNumberSearch) currentEccourededNumber++;

                }
                eccourededNumber.push_back(currentEccourededNumber);
                currentNumberSearch--;
            }
            SearchNumberVector.push_back(eccourededNumber);
        }

        // ������� l1 ... ln, ������� �� �����, ����� ����� S

        QVector<qint32> realNumberPool;  /* ���, � ������� �������� ����� ����� ��� ����������,
                                          * ��� ���������� ��������� = 9 ��� �����: {1,2,3,4,5,6,7,8,9}
                                          */
        QVector<float> vectorS;                      // ������� �������������� ���� ������
        float currentLi = 0.0;                       // ������� ������� �������������� ���� ������
        float currentNumberFromSearchVector = 0;     // ������� ���������� ����������
        float summFromRealNumberPool = 0;            // ������� ����� �� ������� ����� �����
        qint32 currentPositionInRealNumberPool = 0;  // ������� ������� �������� � ���� ����� �����
        float summS = 0.0;                           // ����� ��������������� ������� li
        float S = 0.0;                               // �����

        vectorS.clear();
        realNumberPool.clear();

        for (int i = 0; i < numberExperts; i++)  // ���������� ����
        {
            realNumberPool.push_back(i + 1);
        }

        /* ��� ���������� ����� �������� li. ��� ���������� ������� ���������� ���������� �������
         * ������ �������� (��� �����������), � ��������� - ����� ����������� ����� (�� 1 �� ���-��
         * ���������), �� ���������� ������ �����������, �� ���� ���� ���������� ���� 3, �� � ���������
         * ����� ������: 1 + 2 + 3.
         */

        for (int i = 0; i <  numberIndex; i++)
        {
            summS = 0.0;
            currentPositionInRealNumberPool = 0;

            for (int j = 0; j < SearchNumberVector.at(i).size(); j++)
            {
                currentNumberFromSearchVector = SearchNumberVector.at(i).at(j);  // ����� ���������� ����������
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

        if ( numberIndex == 1)
        {
            // � ����� ������ ����� �����������
            QMessageBox::information(this, "Information", "In table " + QString::number(_tableNumber + 1)
                                     + " expert opinions agreed.");
            return  averageVector;
        }
        else
        {
            float summDiSqrt = 0.0;  // ����� ���������� � �������� ��� �����. ����������� � �������� �������� �������

            for (int i = 0; i < vectorS.size(); i++)
            {
                summDiSqrt += pow((vectorS.at(i) - S), 2);
            }

            float summTi = 0.0;  /* ���������� ������������ ������ �� i-�� ����������
                                  * (����� �������������� �����, ��� ��� ������ ��� ����������� � ����������)
                                  */

            QVector<float> vectorTi;  // �����, ����� ������� � ������� ��������, ���� ���������� �����

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
            // ���� ������ �����������
            QMessageBox::information(this, "Information", "In table " + QString::number(_tableNumber + 1)
                                     + " expert opinions agreed.");
            return  averageVector;

        } else {

            // ������ ������ ���������, ����� ��� �� �����������
            float averageSummVVector = 0.0;  // ������� �������������� ������������� ������������
            QString stringWithPosition;      /* ������ � ���������, ����� ��������� ������� ��� ��������
                                          * ������� ���������, ������� ���������
                                          * ������ �������� ��������������� � �� ���� ���������������
                                          */

            stringWithPosition = "";
            averageSummVVector = getVectorSumm(VVector);
            averageSummVVector /= numberIndex;

            for (qint32 i = 0; i < VVector.size(); i++)  // ������� ����������, ������� ���� ���������������
            {
                if (VVector.at(i) >= averageSummVVector) stringWithPosition += QString::number(i + 1) + " ";
            }

            QMessageBox::warning(this, "Error", "Expert opinions are not agreed. \n Please check index " + stringWithPosition
                                 + "\n in table " + QString::number(_tableNumber + 1));
            VVector.push_back(-42);  // ����, ������������, ��� ������ �� ����������� � ���������� ������� �� ����� ������
            return averageVector;
        }
    }
}

float MethodsHAWC::getHeeSquareTableValue(qint32 _numberIndex, float _nu)
{
    // ������� �������� ��^2
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

    qint32 r;       // ����� ������ � �������
    float p;        // �����������, ��� �������� ������������� ��^2 � �������� r �� ���������� ��� ��������
    qint32 _column; // ����� ������� � �������

    r = _numberIndex - 1 - 1; // ������ "-1" �� �������, ������ "-1", ������ ��� ������� �� 0
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

QString MethodsHAWC::assessProspects(float _ktu)
{
    QString prospectsResult;

    if (_ktu < 1.065 )                 return prospectsResult = "Not promising";
    if (_ktu > 1.065  && _ktu < 1.13 ) return prospectsResult = "Unpromising";
    if (_ktu > 1.13   && _ktu < 1.27 ) return prospectsResult = "Promising";
    if (_ktu > 1.27  )                 return prospectsResult = "Very promising"
                                                                "";
}

/// ---------------------  ������� ��� ���  ---------------------

void MethodsHAWC::calculationUsingHierarchyAnalysisMethod()
{
    QVector<float> integralPrioritets;         // ������ ��������� ���������� �� ������������ �����������
    QVector<QVector<float>> unitsPrioritets;   // ������ ��������� ���������� �� ��������� �����������
    QVector<QVector<float>*> valuesPrioritets; // ������ ��������� ���������� �� �������� ���������������
    QVector<QPair<qint32, float>> finalPrioritets;      // ������ �������� ���������� � ��������������� ��������� �����������

    for (qint32 _table = 0; _table < vectorWithVectorExperts.size(); _table++)
    {
        if (_table == 0)
        {
            if (NumberIntegralIndex == 1)
            {
                integralPrioritets = calculationLocalPrioritets(getMatrixPairedComparsion(vectorWithVectorExperts.at(_table).at(0)), 1, 1, _table);
            } else {
                integralPrioritets = calculationLocalPrioritets(getMatrixPairedComparsion(vectorWithVectorExperts.at(_table)), 1, 0, _table);
            }
        }
        else
        {
            if (NumberUnitsIndexVector.at(_table - 1) == 1)
            {
                unitsPrioritets.push_back(calculationLocalPrioritets(getMatrixPairedComparsion(vectorWithVectorExperts.at(_table).at(0)), 1, 1, _table));
            } else {
                unitsPrioritets.push_back(calculationLocalPrioritets(getMatrixPairedComparsion(vectorWithVectorExperts.at(_table)), 1, 0, _table));
            }
        }
    }

    // ���������� ������� �������� ���������� ������ ��������
    // �� ���������� ����������� (����� ������ �� �������� � ��������� ����������������)
    for (qint32 _row = 0; _row < NumberAlternative; _row++)
    {
        QVector<float> *newVector = new QVector<float>;
        valuesPrioritets.push_back(newVector);
    }

    for (qint32 _table = 0; _table < vectorWithVectorValue.size(); _table++)
    {
        for (qint32 _row = 0; _row < vectorWithVectorValue.at(_table).size(); _row++)
        {
            QVector<float> currentLocalPrioritets;  // ������ ��� ������� ��������� ����������� �� ������� ��
            currentLocalPrioritets =  calculationLocalPrioritets(getMatrixPairedComparsion(vectorWithVectorValue.at(_table).at(_row)), 0, 0, _table);

            // ������ � ������ � ��������� ����������� �����������
            // ��������� ����������� �� ������� �� ������� �������
            // (������ ��� ����� ���������� ��������� ����������� ��� ������� ��)
            for (qint32 _row = 0; _row < currentLocalPrioritets.size(); _row++)
            {
                valuesPrioritets.at(_row)->push_back(currentLocalPrioritets.at(_row));
            }

            currentLocalPrioritets.clear();
        }
    }

    QVector<float> multIntegralUnitsPrioritets;  // ������ ������������� �� �� ��������������� ��

    multIntegralUnitsPrioritets.clear();

    for (qint32 _currentIntegralPrioritets = 0; _currentIntegralPrioritets < integralPrioritets.size(); _currentIntegralPrioritets++)
    {
        for (qint32 _currentUnitsPrioritets = 0; _currentUnitsPrioritets < unitsPrioritets.at(_currentIntegralPrioritets).size(); _currentUnitsPrioritets++)
        {
            multIntegralUnitsPrioritets.push_back(integralPrioritets.at(_currentIntegralPrioritets) *
                                                  unitsPrioritets.at(_currentIntegralPrioritets).at(_currentUnitsPrioritets));
        }
    }

    float currentClobalPrioritets = 0;

    for (qint32 _currValuePrioritetsVec = 0; _currValuePrioritetsVec < valuesPrioritets.size(); _currValuePrioritetsVec++)
    {
        currentClobalPrioritets = 0.0;


        for (qint32 _currValuePrioritets = 0; _currValuePrioritets < valuesPrioritets.at(_currValuePrioritetsVec)->size(); _currValuePrioritets++)
        {
            currentClobalPrioritets += valuesPrioritets.at(_currValuePrioritetsVec)->at(_currValuePrioritets) *
                    multIntegralUnitsPrioritets.at(_currValuePrioritets);
        }

        QPair<qint32, float> currentAlternative;
        currentAlternative.first = _currValuePrioritetsVec;
        currentAlternative.second = currentClobalPrioritets;
        finalPrioritets.push_back(currentAlternative);
    }

    ui->finalPrioritetsTableWidget->setRowCount(NumberAlternative);
    ui->finalPrioritetsTableWidget->setColumnCount(2);
    ui->finalPrioritetsTableWidget->setHorizontalHeaderLabels(QStringList() << "#" << "Global prioritets");

    for (qint32 _row = 0; _row < finalPrioritets.size(); _row++)
    {
        QTableWidgetItem *numberAlternative = new QTableWidgetItem(QString::number(finalPrioritets.at(_row).first + 1));
        ui->finalPrioritetsTableWidget->setItem(_row, 0, numberAlternative);
        ui->finalPrioritetsTableWidget->setColumnWidth(0, 60);

        QTableWidgetItem *valueGlobalPrioritets = new QTableWidgetItem(QString::number(finalPrioritets.at(_row).second));
        ui->finalPrioritetsTableWidget->setItem(_row, 1, valueGlobalPrioritets);
        ui->finalPrioritetsTableWidget->setColumnWidth(1, 120);
    }
}

QVector<float> MethodsHAWC::calculationLocalPrioritets(QVector<QVector<float> > _pairedComparsionMatrix,
                                                       bool _flagCheckOpinion,
                                                       bool _flagIfRowExpert,
                                                       qint32 _numberTable)
{
    qint32 numberRows;     // ���-�� ����� ���
    qint32 numberColumns;  // ���-�� �������� ���

    QVector<float> artworkVector;          // ������ ������������ �� ������� ��� (������� ������ ����������)
    QVector<float> rootsVector;            // ������ ������ �� ������������ ���
    QVector<float> localPrioritetsVector;  // ������ ��������� ��������� �����������
    QVector<float> lambdasVector;          // ������ ����� ��� ����������� ��������������
    QVector<float> summColumnsVector;      // ������ ���� �� �������

    float currentColumnSumm;  // ����� ��������� �������� �������
    float currentRowArtwork;  // ������������ ��������� ������� �������� ������
    float lambdaMax;            // ����� ������ ��� ���������� ���������������
    float indexConsistency;     // ������ ���������������
    float relativeConsistency;  // ������������� ���������������
    float randomConsistency;    // ��������� ���������������

    artworkVector.clear();
    rootsVector.clear();
    localPrioritetsVector.clear();
    float summRoots;            // ����� ����� � �����
    lambdasVector.clear();
    summColumnsVector.clear();

    numberRows = _pairedComparsionMatrix.size();
    numberColumns = _pairedComparsionMatrix.at(0).size();

    // ���������� ������� ������������
    for (qint32 _row = 0; _row < numberRows; _row++)
    {
        currentRowArtwork = 1;

        for (qint32 _column = 0; _column < numberColumns; _column++)
        {
            currentRowArtwork *= _pairedComparsionMatrix.at(_row).at(_column);
        }

        artworkVector.push_back(currentRowArtwork);
    }

    // ���������� ����� ����� � ������ �������
    for (qint32 _column = 0; _column < numberColumns; _column++)
    {
        currentColumnSumm = 0;

        for (qint32 _row = 0; _row < numberRows; _row++)
        {
            currentColumnSumm += _pairedComparsionMatrix.at(_row).at(_column);
        }
        summColumnsVector.push_back(currentColumnSumm);
    }

    // ���������� ������ ������������
    for (qint32 _row = 0; _row < numberRows; _row++)
    {
        rootsVector.push_back(pow(artworkVector.at(_row), (1 / static_cast<float>(numberRows))));
    }

    // ������� ����� ������
    summRoots = getVectorSumm(rootsVector);

    // ���������� ��������� �����������
    for (qint32 _row = 0; _row < numberRows; _row++)
    {
        localPrioritetsVector.push_back(rootsVector.at(_row) / summRoots);
    }

    // ���������� ������� �����
    for (qint32 _row = 0; _row < numberRows; _row++)
    {
        lambdasVector.push_back((localPrioritetsVector.at(_row)) * (summColumnsVector.at(_row)));
    }

    // ���������� ������������� �������� ������
    lambdaMax = getVectorSumm(lambdasVector);

    if (_flagCheckOpinion)
    {
        indexConsistency = (lambdaMax - numberRows) / (numberRows - 1);
        randomConsistency = getRandomConsistency(numberRows);
        relativeConsistency = indexConsistency / randomConsistency;

        if (relativeConsistency > 0.1)
        {
            QMessageBox::warning(this, "Error", "In table " + QString::number(_numberTable + 1) +
                                 "\nExperts opinion not agree.");
        } else {
            QMessageBox::information(this, "Information", "In table " + QString::number(_numberTable + 1) +
                                     "\nExperts opinion agree.");
        }
    }

    // ���� �������������� ������� ������ ��������� ��������� ��������, �� ��������� �������� ��������� �����������
    if (_flagIfRowExpert)
    {
        float arithmeticMeanLocalPrioritets;

        arithmeticMeanLocalPrioritets = getArithmeticMean(localPrioritetsVector);
        localPrioritetsVector.clear();
        localPrioritetsVector.push_back(arithmeticMeanLocalPrioritets);
    }

    return localPrioritetsVector;
}

QVector<QVector<float> > MethodsHAWC::getMatrixPairedComparsion(QVector<float> _rowOpinionOrValue)
{
    QVector<QVector<float>> pairedComparsionMatrix;  // ������� ������ ���������
    QVector<float> rowPairedComparsionMatrix;        // ������ ������� ������ ���������

    qint32 numberRowsColumns;  // ���-�� �����, ��� ��� ������, ��� ��� ������� ����������

    pairedComparsionMatrix.clear();
    rowPairedComparsionMatrix.clear();

    numberRowsColumns = _rowOpinionOrValue.size();

    // ��������� ������� ������ ���������
    for (qint32 _row = 0; _row < numberRowsColumns; _row++)
    {
        for (qint32 _column = 0; _column < numberRowsColumns; _column++)
        {
            // ������� ��������� ������ ������������ ���� ����� �� ������� ����������
            rowPairedComparsionMatrix.push_back(_rowOpinionOrValue.at(_column) / _rowOpinionOrValue.at(_row));
        }
        pairedComparsionMatrix.push_back(rowPairedComparsionMatrix);
        rowPairedComparsionMatrix.clear();
    }

    return pairedComparsionMatrix;
}

QVector<QVector<float> > MethodsHAWC::getMatrixPairedComparsion(QVector<QVector<float> > _tableOpinionOrValue)
{
    QVector<QVector<float>> pairedComparsionMatrix;  // ������� ������ ���������
    QVector<float> arithmeticMeanVector;             // ������ ������� �������������� �� ������� �������
    qint32 numberRows;                               // ���-�� ����� � �������

    arithmeticMeanVector.clear();
    pairedComparsionMatrix.clear();

    numberRows = _tableOpinionOrValue.size();

    // ��� ������ ������ ������� ������� ������� �������������� ������
    for (qint32 _row = 0; _row < numberRows; _row++)
    {
        arithmeticMeanVector.push_back(getArithmeticMean(_tableOpinionOrValue.at(_row)));
    }

    // ������� ������� ������ ��������� ��� ��������� ������� ��������������
    pairedComparsionMatrix = getMatrixPairedComparsion(arithmeticMeanVector);

    return pairedComparsionMatrix;
}

float MethodsHAWC::getRandomConsistency(qint32 _matrixSize)
{
    float matrixRandomConsistency[15] =
    {0, 0, 0.58, 0.9, 1.2, 1.24, 1.32, 1.41, 1.45, 1.49, 1.51, 1.54, 1.56, 1.57, 1.59};

    if ((_matrixSize - 1) > 16)
    {
        QMessageBox::warning(this, "Error", "Value matrix size is out of range. \n Random consistency = 1.59");
        return matrixRandomConsistency[15];
    } else {
        return matrixRandomConsistency[_matrixSize - 1];
    }
}

/// ---------------------  ����� �������  -------------------------

/* ��� ������� �� ������ � Layout �������� LineEdit
 * ��� ����� ���������� ��������� �����������,
 * ��������������� ������� ������������� ����������
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
        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // ������ �� LineEdit ����������, ����� � ���� ���� �����
        NumberLineEdit++;
    }
}

// ���� ��� ��������� �������, � ������� �������� ���������� ��������� ����������� ��� ������� �������������
void MethodsHAWC::slotGetNumberUnitsIndex()
{
    QLineEdit *lineEdit = (QLineEdit*) sender();
    NumberUnitsIndexVector.push_back(lineEdit->text().toInt());
}

void MethodsHAWC::on_CreateTablesPushButton_clicked()
{
    if (ui->NumberIntegralIndexLineEdit->text() == "" |
            ui->NumberAlternativeLineEdit->text()   == "" |
            ui->NumberExpertsLineEdit->text()       == "" )
    {
        QMessageBox::warning(this, "Error", "Not all values entered");
    }
    else
    {
        NumberExperts = ui->NumberExpertsLineEdit->text().toInt();
        NumberAlternative = ui->NumberAlternativeLineEdit->text().toInt();

        tableWidgetValueVector.clear();
        tableWidgetExpertVector.clear();

        if (ui->HAMRadioButton->isChecked())
        {
            // ��������� ������� ��� ������ ��������� �� ��
            QTableWidget *integralIndexTableWidgetExperts = new QTableWidget();
            integralIndexTableWidgetExperts->setRowCount(NumberIntegralIndex);
            integralIndexTableWidgetExperts->setColumnCount(NumberExperts);
            resizeColumnWidth(integralIndexTableWidgetExperts);
            ui->ExpertsVerticalLayout->addWidget(integralIndexTableWidgetExperts);
            tableWidgetExpertVector.push_back(integralIndexTableWidgetExperts);
        }

        for (qint32 i = 0; i < NumberIntegralIndex; i++)
        {
            // �������� ������ �������� ��������
            QTableWidget *tableWidgetValue = new QTableWidget();
            tableWidgetValue->setRowCount(NumberUnitsIndexVector.at(i));
            QStringList columnNumbering;

            // ��� ��� +1 ������� ����� ��� ������ ����������� ����� �� (1 - ���������, -1 - ���������)
            if (ui->WCMRadioButton->isChecked())
            {
                columnNumbering << "State TL" << "Basic"; // State technical level (TL)
                for(int i = 0; i < NumberAlternative - 1; i++)
                {
                    columnNumbering << QString::number(i + 1);
                }
                tableWidgetValue->setColumnCount(NumberAlternative + 1);
                tableWidgetValue->setHorizontalHeaderLabels(columnNumbering);

            } else {
                tableWidgetValue->setColumnCount(NumberAlternative);
            }
            resizeColumnWidth(tableWidgetValue);
            //tableWidgetValue->resize(100, 100);
            ui->ValuesVerticalLayout->addWidget(tableWidgetValue);
            tableWidgetValueVector.push_back(tableWidgetValue);

            // �������� ������ ��� ������ ���������
            QTableWidget *tableWidgetExperts = new QTableWidget();
            tableWidgetExperts->setRowCount(NumberUnitsIndexVector.at(i));
            tableWidgetExperts->setColumnCount(NumberExperts);
            resizeColumnWidth(tableWidgetExperts);
            //tableWidgetExperts->resize(100, 100);
            ui->ExpertsVerticalLayout->addWidget(tableWidgetExperts);
            tableWidgetExpertVector.push_back(tableWidgetExperts);
        }
    }
}

QVector<QVector<float>> MethodsHAWC::getQVectorFromQTableWidget(QTableWidget *_tableWidget)
{
    qint32 numberIndex;     // ���-�� �����
    qint32 numberExperts;  // ���-�� ��������
    QVector<QVector<float> > vectorFromTableWidget;  // �������� ������
    QVector <float> vectorRows;  // ������, �������� ������

    numberIndex = _tableWidget->rowCount();
    numberExperts = _tableWidget->columnCount();

    for (qint32 i = 0; i < numberIndex; i++)
    {
        for (qint32 j = 0; j < numberExperts; j++)
        {
            if (_tableWidget->item(i, j)->text() == "")
            {
                QMessageBox::warning(this, "Error", "Not all values entered in tables. \n Check tables.");
                vectorRows.push_back(-42);
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

void MethodsHAWC::saveAsCSV(QString filename, QTableWidget* _tableWidget)
{
    QFile tableFile(filename);

    if( tableFile.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &tableFile );
        QStringList strList;

        strList << "\" \"";
        for( int c = 0; c < _tableWidget->horizontalHeader()->count(); ++c )
            strList << "\"" + _tableWidget->model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < _tableWidget->verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\"" + _tableWidget->model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < _tableWidget->horizontalHeader()->count(); ++c )
            {
                strList << "\"" + _tableWidget->model()->data(_tableWidget->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }

        tableFile.close();
    }
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

float MethodsHAWC::getArithmeticMean(QVector<float> _vector)
{
    return getVectorSumm(_vector) / _vector.size();
}

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

void MethodsHAWC::on_SavePushButton_clicked()
{
    saveAsCSV("PriorityTable", ui->finalPrioritetsTableWidget);
}
