#include "MainWindow.h"
#include <QApplication>
#include <QWidget>
#include <QTextCodec>

// ������ ������������� ������ ������������ ������ ������� ����������� �������
// Methods of comparative assessments of the technical level of a complex technical system

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
