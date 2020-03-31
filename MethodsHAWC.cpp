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

    // �������� MenuBar

    openMainWindowAction = new QAction("&Choose method", this);
    connect(openMainWindowAction, SIGNAL(triggered()), this, SLOT(onOpenMainWindowClicked()));
    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = this->menuBar()->addMenu("&File");

    fileMenu->addAction(openMainWindowAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    NumberLineEdit = 0;


    ui->tableWidget->setRowCount(3);  // �����������
    ui->tableWidget->setColumnCount(5);  // ���������
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
    fillVectorsWithValuesFromTableWidget();  // ��������� ������� ��� ��������

    // ��� ���������� ����������� ������ ������� � ��� ������

    if (ui->HAMRadioButton->isChecked())  // ���� ���
    {

    }
    else if (ui->WCMRadioButton->isChecked())  // ���� ���
    {
        HeeSquareValueColumn =  ui->PforHeeSquareLineEdit->text().toInt();  // ����� ������� ��� ��������� ��^2

        allAverageVector.clear();

        for (qint32 _table = 0; _table < vectorWithVectorExperts.size(); _table++)
        {
            qint32 counterColumn = 2;
            float averageCurrentSumm = 0.0;  // ����� ������� �������������� ��� ������� �������
            QVector<float> averageForCurrentTable;  // ������ ������� �������������� ��� ������� �������

            averageForCurrentTable = checkOpinionAndGetAverageVector(vectorWithVectorExperts.at(_table), HeeSquareValueColumn, _table);
            averageCurrentSumm = getVectorSumm(averageForCurrentTable);
            allAverageVector.push_back(averageCurrentSumm);  // ���������� ����� ������� �������������� ��� ���������

            while (counterColumn <= vectorWithVectorExperts.at(_table).at(0).size()) // ���� �� ��������
            {
                float kpi = 0.0;  // �������� ��i ��� �������� �������

                for (qint32 _row = 0; _row < vectorWithVectorExperts.at(_table).size(); _row++)  // ���� �� ������� � �������
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

        qDebug() << "kpiFiTableVector = " << kpiFiTableVector;
        qDebug() <<"allAverageVector = " << allAverageVector;

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
            currentKtu = currentKpiFiSumm / resultAverageSumm;
            ktuVector.push_back(currentKtu);
            currentColumn++;
        }

        qDebug() << "ktuVector = " << ktuVector;
    }
}

/* ��� ������� �� ������ � Layout �������� LineEdit ��� ����� ���������� ��������� �����������,
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


    //    ui->IntegralIndexVerticalLayout->addWidget(lineEdit);  // ��������� ������ � ���� � ������������ �����������
    //    lineEdit->setPlaceholderText(QString::number(NumberLineEdit + 1) + " Integral Index");
    //    NumberLineEdit++;
    //    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // ������ �� LineEdit ����������, ����� � ���� ���� �����

    //ui->scrollArea->setWidgetResizable(true);

    //    for (int k = 0; k < NumberIntegralIndex; k++)
    //    {
    //        // DynamicLineEdit *lineEdit = new DynamicLineEdit();  // ������� ������ ������������ ������
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

    //        ui->IntegralIndexVerticalLayout->addWidget(lineEdit);  // ��������� ������ � ���� � ������������ �����������
    //        lineEdit->setPlaceholderText(QString::number(NumberLineEdit + 1) + " Integral Index");
    //        NumberLineEdit++;
    //        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(slotGetNumberUnitsIndex()));  // ������ �� LineEdit ����������, ����� � ���� ���� �����
    //    }
}

// ���� ��� ��������� �������, � ������� �������� ���������� ��������� ����������� ��� ������� �������������
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
        // ��������� ������� ��� ������ ��������� �� ��
        QTableWidget *integralIndexTableWidgetExperts = new QTableWidget();
        integralIndexTableWidgetExperts->setRowCount(NumberIntegralIndex);
        integralIndexTableWidgetExperts->setColumnCount(NumberExperts);
        ui->ExpertsVerticalLayout->addWidget(integralIndexTableWidgetExperts);
        tableWidgetExpertVector.push_back(integralIndexTableWidgetExperts);
    }

    for (qint32 i = 0; i < NumberIntegralIndex; i++)
    {
        // �������� ������ �������� ��������
        QTableWidget *tableWidgetValue = new QTableWidget();
        tableWidgetValue->setRowCount(NumberUnitsIndexVector.at(i));
        tableWidgetValue->setColumnCount(NumberAlternative + 1);  // + 1 ������� ����� ��� ������ ����������� ����� �� (1 - ���������, 0 - ���������)
        tableWidgetValue->resize(100, 100);
        ui->layout->addWidget(tableWidgetValue);
        tableWidgetValueVector.push_back(tableWidgetValue);

        // �������� ������ ��� ������ ���������
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
            vectorRows.push_back(_tableWidget->item(i, j)->text().toFloat());
        }
        vectorFromTableWidget.push_back(vectorRows);
        vectorRows.clear();
    }

    return  vectorFromTableWidget;
}

QVector<float> MethodsHAWC::checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber, qint32 _tableNumber)
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
    float theoreticalValueHeeSquare = getHeeSquareTableValue(numberIndex, _columnNumber);  // ���� �� �������

    averageVector.clear();
    SCOVector.clear();
    VVector.clear();

    /* ���� ���������� ����������� = 1, �� � ������� ��^2 ����� ������� �� 0,
     * ��� �����������, ������� �������, ��� ������������ ����� 0
     */
    if (numberIndex == 0 | numberExperts == 0)
    {
        QMessageBox::warning(this, "Warning!", "���������� ��������� ��� ����������� ����� 0!");
    }
    else if ( numberIndex == 1)
    {
        omega = 0;
        actualValueHeeSquare = 0;

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

        for (qint32 i = 0; i < VVector.size(); i++)
        {
            averageSummVVector += VVector.at(i);
        }
        averageSummVVector /= numberIndex;

        for (qint32 i = 0; i < VVector.size(); i++)  // ������� ����������, ������� ���� ���������������
        {
            if (VVector.at(i) >= averageSummVVector) stringWithPosition += QString::number(i + 1) + " ";
        }

        QMessageBox::information(this, "Error", "Expert opinions are not agreed. \n Please check index " + stringWithPosition
                                 + "\n in table " + QString::number(_tableNumber + 1));
        VVector.push_back(-42);
        // VVector[0] = -42;  // ����, ������������, ��� ������ �� ����������� � ���������� ������� �� ����� ������
        return VVector;
    }
}

float MethodsHAWC::getHeeSquareTableValue(int n, int p)
{
    int   r   = n - 1 - 1;  // ������ "-1" �� �������, ������ "-1", ������ ��� ������� �� 0
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

void MethodsHAWC::saveAsCSV(QString filename, QTableWidget _tableWidget)  // ���������� ������� � .SCV ������
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


