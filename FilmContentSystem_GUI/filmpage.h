#ifndef FILMPAGE_H
#define FILMPAGE_H

#include "FilmContentSystemApplication.h"
#include <QWidget>

namespace Ui {
class FilmPage;
}

class FilmPage : public QWidget
{
    Q_OBJECT

public:
    explicit FilmPage(const FilmContentSystemApplication *_app, QWidget *parent = nullptr);
    ~FilmPage();

public slots:
    void setId(int id);

private:
    Ui::FilmPage *ui;
    const FilmContentSystemApplication* app;
    int id;
};

extern QString info2String(const FilmInfo& info);

#endif // FILMPAGE_H
