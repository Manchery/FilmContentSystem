#include "retrievepage.h"
#include "ui_retrievepage.h"
#include "common_gui.h"
#include "clickablelabel.h"
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
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

    // 检索结果
    Vector<std::pair<int, std::pair<int, int>>> res = app->retrieve(keywords);
    if (res.size()){
        for (int i = 0; i < res.size(); i++)
            resultLayout->addWidget(retrieResultItem(res[i]));
    }else{
        resultLayout->addWidget(new QLabel(QStringLiteral("未检索到相关结果"),
                ui->scrollAreaWidget));
    }
    resultLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));
}

QWidget *RetrievePage::retrieResultItem(std::pair<int, std::pair<int, int> > res)
{
    // 结构：
    // superItem(superLayout): postLabel + item
    // item(layout): childItem + absLabel
    // childItem(childLayout): nameLabel+cntLabel

    int id = res.first, cnt = res.second.first, tot = res.second.second;

    auto superItem = new QWidget(ui->scrollAreaWidget);
    auto superLayout = new QHBoxLayout(superItem);

    auto item = new QWidget(superItem);
    auto childItem = new QWidget(item);

    auto nameLabel = new ClickableLabel(childItem);
    QString nameStr = "<big><u><font color=\"#1a0dab\">"+CharString2QString(app->getInfo(id).name())+"</font></u></big>";
    for (auto p=keywords.begin();p!=keywords.end();++p){
        QString word = CharString2QString(*p);
        nameStr.replace(word, "</font><font color=\"#dd4b39\">"+word+"</font><font color=\"#1a0dab\">");
    }
    nameLabel->setText(nameStr);
    auto cntLabel = new QLabel(childItem);
    cntLabel->setAlignment(Qt::AlignRight);
    cntLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    cntLabel->setText(QStringLiteral("<font color=\"#70757a\"><small>出现关键词: %1 个, 出现总数: %2 次</small></font>").arg(
            QString::number(cnt), QString::number(tot)));
    auto childLayout = new QHBoxLayout(childItem);
    childLayout->addWidget(nameLabel); childLayout->addWidget(cntLabel);

    auto absLabel = new QLabel(item);
    absLabel->setText(abstract(
            CharString2QString(app->getInfo(id).introduction()), keywords));
    absLabel->setWordWrap(true);
    auto layout = new QVBoxLayout(item);
    layout->addWidget(childItem); layout->addWidget(absLabel);

    auto *postLabel = new QLabel(item);
    QString targetPost = QString::fromStdWString(app->getInputDir())
            + "/poster/" + QString::number(id)+".jpg";
    QPixmap post(targetPost);
    postLabel->setPixmap(post.scaled(post.size()*0.4));

    superLayout->addWidget(postLabel);
    superLayout->addWidget(item);

    connect(nameLabel, &ClickableLabel::clicked, [id, this](){
        emit filmClicked(id, keywords);
    });

    return superItem;
}

QString RetrievePage::abstract(const QString &text, const CharStringLink &keywords)
{
    const int MaxLen = 200, Span = 15;
    int *flag = new int[text.length()+1];
    memset(flag, 0, sizeof(int)*(text.length()+1));
    for (auto p = keywords.begin();p!=keywords.end();++p){
        QString word = CharString2QString(*p);
        int last = -1;
        while (true){
            int pos = text.indexOf(word,last+1);
            if (pos ==-1) break;
            flag[std::max(pos-Span,0)]++;
            flag[std::min(pos+Span,text.length())]--;
            last = pos;
        }
    }
    QString abs;
    for (int i=0;i<text.length() && abs.length()<MaxLen;i++){
        if (i) flag[i]+=flag[i-1];
        if (flag[i])
            abs+=text[i];
        else if (i==0 || flag[i-1])
            abs+="...";
    }
    if (!abs.endsWith("...")) abs += "...";
    abs.replace("\n", "");
    for (auto p=keywords.begin();p!=keywords.end();++p){
        QString word = CharString2QString(*p);
        abs.replace(word, "<font color=\"#dd4b39\">"+word+"</font>");
    }
    if (abs == "...")
        abs = text.mid(0,60) + "...";
    return abs;
}

void RetrievePage::setText(QString text)
{
    ui->keywordsEdit->setText(text);
}

void RetrievePage::on_searchButton_clicked()
{
    QString keywords_raw = ui->keywordsEdit->text();
    QStringList splitted = keywords_raw.split(' ', QString::SkipEmptyParts);
    CharStringLink keywords;
    for (QString keyword: splitted){
        keywords.concat(app->divideWords(keyword.toStdWString(), app->getUseHMM(), app->getUseStopwords()));
    }
    if (keywords.empty()){
        QMessageBox::warning(this, QStringLiteral("关键词不足"), QStringLiteral("请输入更多的关键词（非停用词）"));
        return;
    }
    setText(keywords_raw);
    retrieve(keywords);

    emit keywordsChanged(keywords_raw);
}

void RetrievePage::on_keywordsEdit_returnPressed()
{
    on_searchButton_clicked();
}

