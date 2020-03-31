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

    // Сохранение в .SCV формат
    void saveAsCSV(QString filename, QTableWidget _tableWidget);

    // Получение вектора с данными из таблицы
    QVector< QVector<float> > getQVectorFromQTableWidget(QTableWidget *_tableWidget);

    // Вектор средних арифметических для МВК
    QVector<float> checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber, qint32 _tableNumber);

    // Получение значения Хи^2, где n - кол-во показателей, p - номер столбца, соответсвующий (1 - mu)
    float getHeeSquareTableValue(int n, int p);

    // Заполенение двух векторов векторами, полученными из таблиц, для дальнейших вычислений
    void fillVectorsWithValuesFromTableWidget();

    // Посчет суммы чисел в векторе
    float getVectorSumm(QVector<float> _dataVector);

signals:
    void openMainWindow();

private slots:
    void onOpenMainWindowClicked();                 // Вызывает главное окно
    void slotGetNumberUnitsIndex();                 // Получения количества единичных показателей
    void on_CreateTablesPushButton_clicked();       // Создает необходимое количество таблиц
    void on_CreateSingleIndexPushButton_clicked();  // Создает необходимое количество LineEdit для каждого ИП

    void on_TEST_BTN_clicked();

    void on_CalculationPushButton_clicked();

private:
    Ui::MethodsHAWC *ui;

    QMenu *fileMenu;                //  Указатель на меню
    QAction *openMainWindowAction;  // Открыть главное окно
    QAction *exitAction;            //  Выйти

    qint32 NumberLineEdit;                   // Номер LineEdit, для отображения в нем и удобного заполнения
    qint32 NumberIntegralIndex;              // Количество интегральных показателей
    qint32 NumberExperts;                    // Кол-во экспертов
    qint32 NumberAlternative;                // Кол-во альтернатив
    qint32 HeeSquareValueColumn;             // Номер столбца в таблице Хи^2
    QVector<qint32> NumberUnitsIndexVector;  // Вектор, хранящий количество единичных показателей

    QVector<QTableWidget*> tableWidgetValueVector;   // Хранит созданные табллицы с числовыми харакетристиками
    QVector<QTableWidget*> tableWidgetExpertVector;  // Хранит созданные таблицы с оценками экспертов

    QVector<QVector<QVector<float>>> vectorWithVectorValue;    // Хранит вектор из переведенных в веторы таблиц со значениями
    QVector<QVector<QVector<float>>> vectorWithVectorExperts;  // Хранит вектор из переведенных в векторы таблиц с оценками экспертов
    QVector<float> allAverageVector;                           // Хранит сумму средних арифметических для каждой таблицы по каждому показателю

    QVector<float> kpiFiRowVector;             // Строка со значениями Кпi
    QVector<QVector<float>> kpiFiTableVector;  // Таблица со строками, хранящимим значения Кпi
    QVector<float> ktuVector;                  // Содержит значния Кту для каждой альтерантивы

    // Таблица значений Хи^2
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
