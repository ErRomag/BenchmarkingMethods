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

    // Изменяет ширину ячейки, чтобы было удобнее работать с таблицами
    void resizeColumnWidth(QTableWidget *_tableWidget);

    ///  -------------  МВК ---------------
    // Функция оценки перспетивности - Метод весовых коэффициентов
    void calculationUsingWeightCoefficientMethod();

    // Вектор средних арифметических для МВК
    QVector<float> checkOpinionAndGetAverageVector(QVector<QVector<float>> _vectorFromTableWidget, qint32 _columnNumber, qint32 _tableNumber);

    // Получение значения Хи^2, где n - кол-во показателей, p - номер столбца, соответсвующий (1 - mu)
    float getHeeSquareTableValue(int n, int p);

    // Заполенение двух векторов векторами, полученными из таблиц, для дальнейших вычислений
    void fillVectorsWithValuesFromTableWidget();

    // Посчет суммы чисел в векторе
    float getVectorSumm(QVector<float> _dataVector);

    // Функция оценки перспективности образца СТС
    QString assessProspects(float _ktu);

signals:
    void openMainWindow();

private slots:
    void onOpenMainWindowClicked();                 // Вызывает главное окно
    void slotGetNumberUnitsIndex();                 // Получения количества единичных показателей
    void on_CreateTablesPushButton_clicked();       // Создает необходимое количество таблиц
    void on_CreateSingleIndexPushButton_clicked();  // Создает необходимое количество LineEdit для каждого ИП
    void on_CalculationPushButton_clicked();        // Производит все вычисления

    void on_TEST_BTN_clicked();

private:
    Ui::MethodsHAWC *ui;

    QMenu *fileMenu;                //  Указатель на меню
    QAction *openMainWindowAction;  // Открыть главное окно
    QAction *exitAction;            //  Выйти

    qint32 NumberLineEdit;                   // Номер LineEdit, для отображения в нем и удобного заполнения
    qint32 NumberIntegralIndex;              // Количество интегральных показателей
    qint32 NumberExperts;                    // Кол-во экспертов
    qint32 NumberAlternative;                // Кол-во альтернатив

    QVector<qint32> NumberUnitsIndexVector;  // Вектор, хранящий количество единичных показателей

    QVector<QTableWidget*> tableWidgetValueVector;   // Хранит созданные табллицы с числовыми харакетристиками
    QVector<QTableWidget*> tableWidgetExpertVector;  // Хранит созданные таблицы с оценками экспертов

    QVector<QVector<QVector<float>>> vectorWithVectorValue;    // Хранит вектор из переведенных в веторы таблиц со значениями
    QVector<QVector<QVector<float>>> vectorWithVectorExperts;  // Хранит вектор из переведенных в векторы таблиц с оценками экспертов
};

#endif // METHODSHAWC_H
