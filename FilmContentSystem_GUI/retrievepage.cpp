#include "retrievepage.h"
#include "ui_retrievepage.h"
#include "common_gui.h"
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <iostream>
#include <QScrollArea>
#include <QMessageBox>

RetrievePage::RetrievePage(FilmContentSystemApplication *_app, QWidget *parent) :
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
    // 清空列表
    QLayoutItem *item;
    while ((item = resultLayout->takeAt(0)) != nullptr){
        delete item->widget(); delete item;
    }

    // 显示检索关键字
    keywords = _keywords;
    QString keywordsText = QStringLiteral("检索关键字:");
    for (auto p = keywords.begin();p!=keywords.end();++p){
        keywordsText += " " + CharString2QString(*p);
    }
    ui->keywordsLabel->setText(keywordsText);

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

void RetrievePage::on_searchButton_clicked()
{
    QString _keywords = ui->keywordsEdit->text();
    QStringList splitted = _keywords.split(' ', QString::SkipEmptyParts);
    CharStringLink keywords;
    for (QString keyword: splitted){
        keywords.concat(app->divideWords(keyword.toStdWString(), app->getUseHMM(), app->getUseStopwords()));
    }
    if (keywords.empty()){
        QMessageBox::warning(this, QStringLiteral("关键词不足"), QStringLiteral("请输入更多的关键词"));
        return;
    }
    setText(_keywords);
    retrieve(keywords);

    emit keywordsChanged(_keywords);
}

void RetrievePage::on_keywordsEdit_returnPressed()
{
    on_searchButton_clicked();
}
