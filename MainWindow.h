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
    void aboutProgramm();  // Справка о программе

    void on_ButtonPriorityTechnology_clicked();
    void on_ButtonHAWCM_clicked();

private:
    Ui::MainWindow *ui;
    const QSize IcoBtnSize = QSize(64, 64);  // Размеры для иконки и кнопки
    //  QSharedPointer<WCMethod> wcMethod;       // Экземпляр класса для МВК
    //  QSharedPointer<HAMethod> haMethod;       // Экземпляр класса для МАИ


    PriorityTechnology *priorityTechnologyWindow;
    MethodsHAWC *methodsHAWC;

    QMenu *fileMenu;       //  Указатель на меню
    QAction *exitAction;   //  Выйти
    QMenu *helpMenu;       //  Помощь
    QAction *aboutAction;  //  О программе
};

#endif // MAINWINDOW_H
