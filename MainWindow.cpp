#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QWidget>

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmapMW(":/Icon/IconMainWindowGrad.png");
    QIcon MWIcon(pixmapMW);
    setWindowIcon(MWIcon);
    setWindowTitle("Methods of comparative assessments");

    // MenuBar
    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    aboutAction = new QAction("&About programm");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutProgramm()));

    fileMenu = this->menuBar()->addMenu("&File");
    helpMenu = this->menuBar()->addMenu("&Help");

    fileMenu->addAction(exitAction);
    helpMenu->addAction(aboutAction);

    QPixmap pixmapHAM(":/Icon/IconHAMGrad.png");
    QIcon ButtonHAMIcon(pixmapHAM);
    ui->ButtonHAWCM->setIcon(QIcon(ButtonHAMIcon));
    ui->ButtonHAWCM->setIconSize(IcoBtnSize);
    ui->ButtonHAWCM->setFixedSize(IcoBtnSize);

    QPixmap pixmapPriority(":/Icon/IconPriorityTechGrad.png");
    QIcon ButtonPriorityIcon(pixmapPriority);
    ui->ButtonPriorityTechnology->setIcon(QIcon(ButtonPriorityIcon));
    ui->ButtonPriorityTechnology->setIconSize(IcoBtnSize);
    ui->ButtonPriorityTechnology->setFixedSize(IcoBtnSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aboutProgramm()
{
    QMessageBox aboutProgrammBox;
    QPixmap pixmapAboutProgramm(":/Icon/IconInfoDGrad.png");
    QIcon AboutProgrammIcon(pixmapAboutProgramm);
    aboutProgrammBox.setWindowIcon(AboutProgrammIcon);

    aboutProgrammBox.setWindowTitle("About programm");
    aboutProgrammBox.setText("The program implements various methods of comparative evaluations of complex technical systems");
    aboutProgrammBox.setInformativeText("Designed by Erg Roman.\nVersion 0.0.1");
    aboutProgrammBox.exec();
}

void MainWindow::on_ButtonPriorityTechnology_clicked()
{
    priorityTechnologyWindow = new PriorityTechnology();
    connect(priorityTechnologyWindow, &PriorityTechnology::openMainWindow, this, &MainWindow::show);

    priorityTechnologyWindow->showMaximized();
    this->close();
}

void MainWindow::on_ButtonHAWCM_clicked()
{
    methodsHAWC = new MethodsHAWC();
    connect(methodsHAWC, &MethodsHAWC::openMainWindow, this, &MainWindow::show);

    methodsHAWC->showMaximized();
    this->close();
}
