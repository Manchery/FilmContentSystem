#ifndef RETRIEVEPAGE_H
#define RETRIEVEPAGE_H

#include "CharStringLink.h"
#include "FilmContentSystemApplication.h"
#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class RetrievePage;
}

class RetrievePage : public QWidget
{
    Q_OBJECT

public:
    explicit RetrievePage(const FilmContentSystemApplication *_app, QWidget *parent = nullptr);
    ~RetrievePage();

public slots:
    void retrieve(const CharStringLink& _keywords);
    void setText(QString text);

private:
    Ui::RetrievePage *ui;
    const FilmContentSystemApplication* app;
    CharStringLink keywords;
    QVBoxLayout *resultLayout;
};

#endif // RETRIEVEPAGE_H
