#include "DynamicLineEdit.h"

DynamicLineEdit::DynamicLineEdit(QWidget *parent) : QLineEdit(parent)
{
    numberLineEdit++;             // Увеличение счетчика на единицу

    lineEditID = numberLineEdit;  /* Присвоение кнопке номера, по которому будет производиться
                                   * дальнейшая работа с кнопок
                                   * */
}

DynamicLineEdit::~DynamicLineEdit()
{

}

/* Метод для возврата значения номера кнопки
 * */
int DynamicLineEdit::getIDLineEdit()
{
    return lineEditID;
}

/* Инициализация статической переменной класса.
 * Статическая переменная класса должна инициализироваться в обязательном порядке
 * */
int DynamicLineEdit::numberLineEdit = 0;
