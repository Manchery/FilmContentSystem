#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FilmContentSystemApplication.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FilmContentSystemApplication().run("C:/Users/Manchery/source/repos/FilmContentSystem/Debug/FilmContentSystem.config");
}

MainWindow::~MainWindow()
{
    delete ui;
}
