#include "filmpage.h"
#include "ui_filmpage.h"
#include <QFile>
#include <sstream>

FilmPage::FilmPage(const FilmContentSystemApplication *_app, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilmPage)
{
    ui->setupUi(this);
    app = _app;
//    ui->infoLabel->setWordWrap(true);
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
        QPixmap img(postPath);
        ui->posterLabel->setPixmap(img);
    }else{
        ui->posterLabel->setVisible(false);
    }
    //info
    ui->infoLabel->setText(info2String(app->getInfo(id)));
}

static QString CharString2QString(const CharString & cstr){
    return QString::fromStdWString(cstr.toWString());
}

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
    if (!info.introduction().empty())
        res += QStringLiteral("<b>剧情简介</b>: ") + "<br>"
                + CharString2QString(info.introduction()) + "<br>";

    return res;
}
