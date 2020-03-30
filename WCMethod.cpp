#include "WCMethod.h"
#include "ui_WCMethod.h"

WCMethod::WCMethod(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WCMethod)
{
    ui->setupUi(this);
    setWindowTitle("Weighting coefficients method");

    QPixmap pixmapWCMWindow(":/Icon/IconWCMGrad.png");
    QIcon WCMWindowIcon(pixmapWCMWindow);
    setWindowIcon(WCMWindowIcon);

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

WCMethod::~WCMethod()
{
    delete ui;
}

void WCMethod::onOpenMainWindowClicked()
{
    this->close();
    emit openMainWindow();
}
