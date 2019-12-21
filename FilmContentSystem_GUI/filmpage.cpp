#include "common_gui.h"
#include "filmpage.h"
#include "ui_filmpage.h"
#include "clickablelabel.h"
#include <QFile>
#include <QScrollArea>
#include <QScrollBar>
#include <algorithm>

FilmPage::FilmPage(const FilmContentSystemApplication *_app, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilmPage)
{
    ui->setupUi(this);
    app = _app;
//    ui->infoLabel->setWordWrap(true);
    recommLayout = new QHBoxLayout(ui->scrollAreaWidget);
}

FilmPage::~FilmPage()
{
    delete ui;
}

void FilmPage::setId(int id)
{
    // poster
    QString postPath = QString::fromStdWString(app->getInputDir()) + "/poster/" + QString::number(id)+".jpg";
    if (QFile::exists(postPath)){
        ui->posterLabel->setPixmap(QPixmap(postPath));
    }else{
        ui->posterLabel->setVisible(false);
    }

    // info
    ui->infoLabel->setText(info2String(app->getInfo(id)));

    // recommend
    Vector<std::pair<int, CharString>> recomm = app->recommend(id, 10);

    // 清空推荐列表
    QLayoutItem *item;
    while ((item = recommLayout->takeAt(0)) != nullptr){
        delete item->widget(); delete item;
    }

    int maxHeight = 0; const double postScale = 0.6;
    for (int i=0;i<recomm.size();i++){
        int targetId = recomm[i].first;

        QWidget *item = new QWidget(ui->scrollAreaWidget);
        QVBoxLayout *layout = new QVBoxLayout(item);
        recommLayout->addWidget(item);

        // 电影海报
        QString targetPost = QString::fromStdWString(app->getInputDir())
                + "/poster/" + QString::number(targetId)+".jpg";
        int postHeight=0;
        if (QFile::exists(targetPost)){
            auto *postLabel = new ClickableLabel(item);
            QPixmap post(targetPost);
            post = post.scaled(post.size()*postScale);
            postLabel->setPixmap(post);
            layout->addWidget(postLabel);
            postHeight=post.height();
            connect(postLabel, &ClickableLabel::clicked,
                    [targetId, this](){ setId(targetId); });
        }

        // 电影名
        auto *nameLabel = new ClickableLabel(item);
        nameLabel->setText("<font color=\"#3377aa\">"+CharString2QString(app->getInfo(targetId).name())+"</font>");
        nameLabel->setAlignment(Qt::AlignHCenter);
        connect(nameLabel, &ClickableLabel::clicked,
                [targetId, this](){ setId(targetId); });
        nameLabel->setWordWrap(true);

        layout->addWidget(nameLabel);
        maxHeight = std::max(maxHeight, postHeight+150);
    }
    ui->recommScrollArea->setMinimumHeight(maxHeight);

    emit filmChanged(CharString2QString(app->getInfo(id).name()));
}

void FilmPage::setHighlight(const CharStringLink &keywords)
{
    QString text = ui->infoLabel->toHtml();
    for (auto p=keywords.begin();p!=keywords.end();++p){
        QString word = CharString2QString(*p);
        text.replace(word, "<span style=\"background-color: #FFFF00\">"+word+"</span>");
        // text.replace(word, "<font color=\"#dd4b39\">"+word+"</font>");
    }
    ui->infoLabel->setText(text);
}

// 将 FilmInfo 类型转换成 html 格式的文本
QString info2String(const FilmInfo& info){
    QString res;
    res += "<big><span>" +CharString2QString(info.name())+ "</span></big><br>";
    if (info.rating()!=-1)
        res += QStringLiteral("<b>评分</b>: ")
                + QString::number(info.rating(),'f',1) + "<br><br>";
    if (!info.directors().empty())
        res += QStringLiteral("<b>导演</b>: ")
                + CharString2QString(info.directors().toCharString()) + "<br>";
    if (!info.screenwriters().empty())
        res += QStringLiteral("<b>编剧</b>: ")
                + CharString2QString(info.screenwriters().toCharString()) + "<br>";
    if (!info.stars().empty())
        res += QStringLiteral("<b>主演</b>: ")
                + CharString2QString(info.stars().toCharString()) + "<br>";
    if (!info.genres().empty())
        res += QStringLiteral("<b>类型</b>: ")
                + CharString2QString(info.genres().toCharString()) + "<br>";
    if (!info.regions().empty())
        res += QStringLiteral("<b>制片国家/地区</b>: ")
                + CharString2QString(info.regions().toCharString()) + "<br>";
    if (!info.languages().empty())
        res += QStringLiteral("<b>语言</b>: ")
                + CharString2QString(info.languages().toCharString()) + "<br>";
    if (!info.dates().empty())
        res += QStringLiteral("<b>上映日期</b>: ")
                + CharString2QString(info.dates().toCharString()) + "<br>";
    if (!info.durations().empty())
        res += QStringLiteral("<b>片长</b>: ")
                + CharString2QString(info.durations().toCharString()) + "<br>";
    if (!info.alternates().empty())
        res += QStringLiteral("<b>又名</b>: ")
                + CharString2QString(info.alternates().toCharString()) + "<br>";
    if (!info.introduction().empty()){
        QString intro = CharString2QString(info.introduction());
        intro = intro.replace("\n","<br>&nbsp;&nbsp;&nbsp;");
        res += QStringLiteral("<br><b>剧情简介</b>: ") + "<br>&nbsp;&nbsp;&nbsp;"
                + intro + "</p><br>";
    }

    return res;
}
