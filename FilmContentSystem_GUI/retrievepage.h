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
    explicit RetrievePage(FilmContentSystemApplication *_app, QWidget *parent = nullptr);
    ~RetrievePage();

signals:
    void keywordsChanged(QString keywords);

public slots:
    void retrieve(const CharStringLink& _keywords);
    void setText(QString text);

private slots:
    void on_searchButton_clicked();

    void on_keywordsEdit_returnPressed();

private:
    Ui::RetrievePage *ui;
    FilmContentSystemApplication* app;
    CharStringLink keywords;
    QVBoxLayout *resultLayout;
};

#endif // RETRIEVEPAGE_H
