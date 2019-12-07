#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "homepage.h"
#include <QTabBar>
#include <QTabWidget>
#include <QMessageBox>
#include <exception>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setWindowState(Qt::WindowMaximized);

    tabs = ui->tabWidget;

    tabs->addTab(homeTab = new HomePage(), QStringLiteral("电影信息检索"));
    tabs->setTabsClosable(true);
    tabs->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    connect(tabs, &QTabWidget::tabCloseRequested, this, [this](int idx){ tabs->removeTab(idx); });

    connect(homeTab, &HomePage::retrieveRequest, this, &MainWindow::retrieve);

    app = new FilmContentSystemApplication();
    QString appConfig;
#ifdef _DEBUG
    appConfig = "C:/Users/Manchery/source/repos/FilmContentSystem/Debug/FilmContentSystem.config";
#else
    appConfig = "FilmContentSystem.config";
#endif
    std::cerr<< appConfig.toStdString().c_str() << std::endl;
    if (!app->init(appConfig.toStdString().c_str())){
        throw std::exception("Database init failed !");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete app;
}

void MainWindow::retrieve(QString _keywords)
{
    QStringList splitted = _keywords.split(' ', QString::SkipEmptyParts);
    CharStringLink keywords;
    for (QString keyword: splitted){
        keywords.push_back(keyword.toStdWString());
    }
    std::wcerr << keywords << std::endl;

    Vector<std::pair<int, std::pair<int, int>>> res = app->retrieve(keywords);

    for (int i = 0; i < res.size(); i++) {
        if (i) std::wcerr << ' ';
        std::wcerr << '(' << res[i].first << ',' << res[i].second.second << ")";
    }
    std::wcerr << std::endl;
}
