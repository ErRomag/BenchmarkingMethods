#ifndef METHODSHAWC_H
#define METHODSHAWC_H

#include <QMainWindow>

#include "DynamicLineEdit.h"
#include <QTableWidget>

namespace Ui {
class MethodsHAWC;
}

class MethodsHAWC : public QMainWindow
{
    Q_OBJECT

public:
    explicit MethodsHAWC(QWidget *parent = nullptr);
    ~MethodsHAWC();

    // ���������� � .SCV ������
    void saveAsCSV(QString filename, QTableWidget _tableWidget);

    // ��������� ������� � ������� �� �������
    QVector< QVector<float> > getQVectorFromQTableWidget(QTableWidget *_tableWidget);

    // �������� ������ ������, ����� ���� ������� �������� � ���������
    void resizeColumnWidth(QTableWidget *_tableWidget);

    ///  -------------  ��� ---------------
    // ������� ������ �������������� - ����� ������� �������������
    void calculationUsingWeightCoefficientMethod();

    // ������ ������� �������������� ��� ���
    QVector<float> checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber, qint32 _tableNumber);

    // ��������� �������� ��^2, ��� n - ���-�� �����������, p - ����� �������, �������������� (1 - mu)
    float getHeeSquareTableValue(int _numberIndex, int _nu);

    // ����������� ���� �������� ���������, ����������� �� ������, ��� ���������� ����������
    void fillVectorsWithValuesFromTableWidget();

    // ������ ����� ����� � �������
    float getVectorSumm(QVector<float> _dataVector);

    // ������� ������ ��������������� ������� ���
    QString assessProspects(float _ktu);

    // ��������������� ������� ��� ��������� ����� � ��������� ������
    bool equalFloatingValue(float _value1, float _value2, float _epsilon);

signals:
    void openMainWindow();

private slots:
    void onOpenMainWindowClicked();                 // �������� ������� ����
    void slotGetNumberUnitsIndex();                 // ��������� ���������� ��������� �����������
    void on_CreateTablesPushButton_clicked();       // ������� ����������� ���������� ������
    void on_CreateSingleIndexPushButton_clicked();  // ������� ����������� ���������� LineEdit ��� ������� ��
    void on_CalculationPushButton_clicked();        // ���������� ��� ����������

    void on_TEST_BTN_clicked();

private:
    Ui::MethodsHAWC *ui;

    QMenu *fileMenu;                //  ��������� �� ����
    QAction *openMainWindowAction;  // ������� ������� ����
    QAction *exitAction;            //  �����

    qint32 NumberLineEdit;                   // ����� LineEdit, ��� ����������� � ��� � �������� ����������
    qint32 NumberIntegralIndex;              // ���������� ������������ �����������
    qint32 NumberExperts;                    // ���-�� ���������
    qint32 NumberAlternative;                // ���-�� �����������

    QVector<qint32> NumberUnitsIndexVector;  // ������, �������� ���������� ��������� �����������

    QVector<QTableWidget*> tableWidgetValueVector;   // ������ ��������� �������� � ��������� ����������������
    QVector<QTableWidget*> tableWidgetExpertVector;  // ������ ��������� ������� � �������� ���������

    QVector<QVector<QVector<float>>> vectorWithVectorValue;    // ������ ������ �� ������������ � ������ ������ �� ����������
    QVector<QVector<QVector<float>>> vectorWithVectorExperts;  // ������ ������ �� ������������ � ������� ������ � �������� ���������
};

#endif // METHODSHAWC_H
