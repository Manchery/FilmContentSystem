#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "homepage.h"
#include "retrievepage.h"
#include "filmpage.h"
#include "common_gui.h"
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

    auto homeTab = new HomePage(tabs);
    tabs->addTab(homeTab, QStringLiteral("电影信息检索"));
    tabs->setTabsClosable(true);
    // homeTab 不可被关闭
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
    }else{
        std::cerr<< "Load Database success !" << std::endl;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete app;
}

void MainWindow::retrieve(QString keywords_raw)
{
    if (app->hasName(keywords_raw.toStdWString())){
        // 主页检索的关键字与电影名完美匹配
        int id = app->getIdFromName(keywords_raw.toStdWString());
        CharStringLink words;
        words.add(keywords_raw.toStdWString());
        newFilmPage(id, words);
    }else{
        QStringList splitted = keywords_raw.split(' ', QString::SkipEmptyParts);
        CharStringLink keywords;
        for (QString keyword: splitted){
            keywords.concat(app->divideWords(keyword.toStdWString(), app->getUseHMM(), app->getUseStopwords()));
        }
        if (keywords.empty()){
            QMessageBox::warning(this, QStringLiteral("关键词不足"), QStringLiteral("请输入更多的关键词（非停用词）"));
            return;
        }
        newRetrievePage(keywords_raw, keywords);
    }
}

void MainWindow::newFilmPage(int id, const CharStringLink &keywords)
{
    auto filmTab = new FilmPage(app, tabs);
    tabs->addTab(filmTab, CharString2QString(app->getInfo(id).name()));
    tabs->setCurrentIndex(tabs->count()-1);

    filmTab->setId(id);
    filmTab->setHighlight(keywords);
    connect(filmTab, &FilmPage::filmChanged,
            [this](QString name){ tabs->setTabText(tabs->currentIndex(),name);});
}

void MainWindow::newRetrievePage(QString keywords_raw, const CharStringLink &keywords)
{
    auto retrieTab = new RetrievePage(app, tabs);
    tabs->addTab(retrieTab, keywords_raw + QStringLiteral(" 的检索结果"));
    tabs->setCurrentIndex(tabs->count()-1);

    retrieTab->setText(keywords_raw);
    retrieTab->retrieve(keywords);

    connect(retrieTab, &RetrievePage::keywordsChanged,
            [this](QString keywords){
                tabs->setTabText(tabs->currentIndex(),keywords + QStringLiteral(" 的检索结果"));
    });
    connect(retrieTab, &RetrievePage::filmClicked, this, &MainWindow::newFilmPage);
}
