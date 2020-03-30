#ifndef PRIORITYTECHNOLOGY_H
#define PRIORITYTECHNOLOGY_H

#include <QMainWindow>

namespace Ui {
class PriorityTechnology;
}

class PriorityTechnology : public QMainWindow
{
    Q_OBJECT

signals:
    void openMainWindow();

public:
    explicit PriorityTechnology(QWidget *parent = nullptr);
    ~PriorityTechnology();

public slots:
    void onOpenMainWindowClicked();

private:
    Ui::PriorityTechnology *ui;

    QMenu *fileMenu;                //  Указатель на меню
    QAction *openMainWindowAction;  // Открыть главное окно
    QAction *exitAction;            //  Выйти
};

#endif // PRIORITYTECHNOLOGY_H
