#ifndef DYNAMICLINEEDIT_H
#define DYNAMICLINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>

class DynamicLineEdit : public QLineEdit
{
    Q_OBJECT
public:

    explicit DynamicLineEdit(QWidget *parent = nullptr);
    ~DynamicLineEdit();
    static qint32 numberLineEdit;  // Статическая переменная, счетчик номеров кнопок
    int getIDLineEdit();  // Функция для возврата локального номера кнопки

private:
    int lineEditID;   // Номер кнопки
};

#endif // DYNAMICLINEEDIT_H
