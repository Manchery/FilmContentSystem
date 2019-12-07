#include "retrievepage.h"
#include "ui_retrievepage.h"
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <iostream>
#include <QScrollArea>

RetrievePage::RetrievePage(const FilmContentSystemApplication *_app, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RetrievePage)
{
    app = _app;
    ui->setupUi(this);
    resultLayout = new QVBoxLayout(ui->scrollAreaWidget);
}

RetrievePage::~RetrievePage()
{
    delete ui;
}

void RetrievePage::retrieve(const CharStringLink &_keywords)
{
    keywords = _keywords;

    std::wcerr << keywords << std::endl;

    Vector<std::pair<int, std::pair<int, int>>> res = app->retrieve(keywords);

    for (int i = 0; i < res.size(); i++) {

        // do sth with res

        if (i) std::wcerr << ' ';
        std::wcerr << '(' << res[i].first << ',' << res[i].second.second << ")";
    }
    std::wcerr << std::endl;
    for (auto p = keywords.begin();p!=keywords.end();++p){
        QString word = QString::fromWCharArray((*p).toWString().c_str());
        resultLayout->addWidget(new QLabel(word,this));
    }
}

void RetrievePage::setText(QString text)
{
    ui->keywordsEdit->setText(text);
}
