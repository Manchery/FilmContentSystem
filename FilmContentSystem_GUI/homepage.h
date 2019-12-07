#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

signals:
    void retrieveRequest(QString keywords);

private slots:
    void on_searchButton_clicked();
    void on_keywordEdit_returnPressed();

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H
