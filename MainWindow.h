#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QWidget>

#include "PriorityTechnology.h"
#include "MethodsHAWC.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void aboutProgramm();  // ������� � ���������

    void on_ButtonPriorityTechnology_clicked();
    void on_ButtonHAWCM_clicked();

private:
    Ui::MainWindow *ui;
    const QSize IcoBtnSize = QSize(64, 64);  // ������� ��� ������ � ������
    //  QSharedPointer<WCMethod> wcMethod;       // ��������� ������ ��� ���
    //  QSharedPointer<HAMethod> haMethod;       // ��������� ������ ��� ���


    PriorityTechnology *priorityTechnologyWindow;
    MethodsHAWC *methodsHAWC;

    QMenu *fileMenu;       //  ��������� �� ����
    QAction *exitAction;   //  �����
    QMenu *helpMenu;       //  ������
    QAction *aboutAction;  //  � ���������
};

#endif // MAINWINDOW_H
