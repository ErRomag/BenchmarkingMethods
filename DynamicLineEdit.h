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
    static qint32 numberLineEdit;  // ����������� ����������, ������� ������� ������
    int getIDLineEdit();  // ������� ��� �������� ���������� ������ ������

private:
    int lineEditID;   // ����� ������
};

#endif // DYNAMICLINEEDIT_H
