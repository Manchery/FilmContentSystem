#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FilmContentSystemApplication.h"
#include "homepage.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void retrieve(QString keywords);

private:
    Ui::MainWindow *ui;

    QTabWidget *tabs;
    HomePage *homeTab;

    FilmContentSystemApplication *app;
};

#endif // MAINWINDOW_H
