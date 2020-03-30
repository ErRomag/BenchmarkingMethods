#include "DynamicLineEdit.h"

DynamicLineEdit::DynamicLineEdit(QWidget *parent) : QLineEdit(parent)
{
    numberLineEdit++;             // ���������� �������� �� �������

    lineEditID = numberLineEdit;  /* ���������� ������ ������, �� �������� ����� �������������
                                   * ���������� ������ � ������
                                   * */
}

DynamicLineEdit::~DynamicLineEdit()
{

}

/* ����� ��� �������� �������� ������ ������
 * */
int DynamicLineEdit::getIDLineEdit()
{
    return lineEditID;
}

/* ������������� ����������� ���������� ������.
 * ����������� ���������� ������ ������ ������������������ � ������������ �������
 * */
int DynamicLineEdit::numberLineEdit = 0;
