#ifndef FILMPAGE_H
#define FILMPAGE_H

#include "FilmContentSystemApplication.h"
#include "CharStringLink.h"
#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
class FilmPage;
}

class FilmPage : public QWidget
{
    Q_OBJECT

public:
    explicit FilmPage(const FilmContentSystemApplication *_app, QWidget *parent = nullptr);
    ~FilmPage();

signals:
    void filmChanged(QString name);

public slots:
    void setId(int id);
    void setHighlight(const CharStringLink& keywords);

private:
    Ui::FilmPage *ui;
    const FilmContentSystemApplication* app;
    int id;
    QHBoxLayout *recommLayout;
};

extern QString info2String(const FilmInfo& info);

#endif // FILMPAGE_H
