#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "homepage.h"
#include "retrievepage.h"
#include "filmpage.h"
#include <QTabBar>
#include <QTabWidget>
#include <QPushButton>
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

    tabs->addTab(homeTab = new HomePage(tabs), QStringLiteral("电影信息检索"));
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
    if (app->hasName(_keywords.toStdWString())){
        int id = app->getIdFromName(_keywords.toStdWString());
        auto filmTab = new FilmPage(app, tabs);
        tabs->addTab(filmTab, _keywords);
        tabs->setCurrentIndex(tabs->count()-1);
        filmTab->setId(id);
        CharStringLink words;
        words.add(_keywords.toStdWString());
        filmTab->setHighlight(words);
        connect(filmTab, &FilmPage::filmChanged,
                [this](QString name){ tabs->setTabText(tabs->currentIndex(),name);});

    }else{
        QStringList splitted = _keywords.split(' ', QString::SkipEmptyParts);
        CharStringLink keywords;
        for (QString keyword: splitted){
            keywords.concat(app->divideWords(keyword.toStdWString(), app->getUseHMM(), app->getUseStopwords()));
        }
        if (keywords.empty()){
            QMessageBox::warning(this, QStringLiteral("关键词不足"), QStringLiteral("请输入更多的关键词"));
        }
        auto retrieTab = new RetrievePage(app, tabs);
        tabs->addTab(retrieTab, _keywords);
        tabs->setCurrentIndex(tabs->count()-1);
        retrieTab->setText(_keywords);
        retrieTab->retrieve(keywords);
    }
}
