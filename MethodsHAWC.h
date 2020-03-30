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

    void saveAsCSV(QString filename, QTableWidget _tableWidget);  // ���������� � .SCV ������
    // ��������� ������� � ������� �� �������
    QVector< QVector<float> > getQVectorFromQTableWidget(QTableWidget *_tableWidget);
    // ������ ������� �������������� ��� ���
    QVector<float> checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber);
    // ��������� �������� ��^2, ��� n - ���-�� �����������, p - ����� �������, �������������� (1 - mu)
    float getHeeSquareTableValue(int n, int p);
    // ����������� ���� �������� ���������, ����������� �� ������, ��� ���������� ����������
    void fillVectorsWithValuesFromTableWidget();

signals:
    void openMainWindow();

private slots:
    void onOpenMainWindowClicked();                 // �������� ������� ����
    void slotGetNumberUnitsIndex();                 // ��������� ���������� ��������� �����������
    void on_CreateTablesPushButton_clicked();       // ������� ����������� ���������� ������
    void on_CreateSingleIndexPushButton_clicked();  // ������� ����������� ���������� LineEdit ��� ������� ��

    void on_TEST_BTN_clicked();

    void on_CalculationPushButton_clicked();

private:
    Ui::MethodsHAWC *ui;

    QMenu *fileMenu;                //  ��������� �� ����
    QAction *openMainWindowAction;  // ������� ������� ����
    QAction *exitAction;            //  �����

    qint32 NumberLineEdit;                   // ����� LineEdit, ��� ����������� � ��� � �������� ����������
    qint32 NumberIntegralIndex;              // ���������� ������������ �����������
    qint32 NumberExperts;                    // ���-�� ���������
    qint32 NumberAlternative;                // ���-�� �����������
    qint32 HeeSquareValueColumn;             // ����� ������� � ������� ��^2
    QVector<qint32> NumberUnitsIndexVector;  // ������, �������� ���������� ��������� �����������

    QVector<QTableWidget*> tableWidgetValueVector;   // ������ ��������� �������� � ��������� ����������������
    QVector<QTableWidget*> tableWidgetExpertVector;  // ������ ��������� ������� � �������� ���������

    QVector<QVector<QVector<float>>> vectorWithVectorValue;  // ������ ������ �� ������������ � ������ ������ �� ����������
    QVector<QVector<QVector<float>>> vectorWithVectorExperts;  // ������ ������ �� ������������ � ������� ������ � �������� ���������

    // ������� �������� ��^2
    float heeSquareTableMatrixNew [30][14]  = {
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
};

#endif // METHODSHAWC_H
