#include "PriorityTechnology.h"
#include "ui_PriorityTechnology.h"

PriorityTechnology::PriorityTechnology(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PriorityTechnology)
{
    ui->setupUi(this);
    setWindowTitle("Priority Technology Ranking");
    QPixmap pixmapPriorityWindow(":/Icon/IconPriorityTechGrad.png");
    QIcon priorityWindowIcon(pixmapPriorityWindow);
    setWindowIcon(priorityWindowIcon);

    // Описание MenuBar

    openMainWindowAction = new QAction("&Choose method", this);
    connect(openMainWindowAction, SIGNAL(triggered()), this, SLOT(onOpenMainWindowClicked()));
    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = this->menuBar()->addMenu("&File");

    fileMenu->addAction(openMainWindowAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

}

PriorityTechnology::~PriorityTechnology()
{
    delete ui;
}

void PriorityTechnology::onOpenMainWindowClicked()
{
    this->close();
    emit openMainWindow();
}
