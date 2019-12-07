#ifndef RETRIEVEPAGE_H
#define RETRIEVEPAGE_H

#include "CharStringLink.h"
#include "FilmContentSystemApplication.h"
#include <QWidget>
#include <QVBoxLayout>
#include <algorithm>

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
    void filmClicked(int id, const CharStringLink &keywords);

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

    QWidget *retrieResultItem(std::pair<int, std::pair<int, int>> res);
    QString abstract(const QString &text, const CharStringLink &keywords);
};

#endif // RETRIEVEPAGE_H
