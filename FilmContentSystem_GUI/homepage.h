#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

/* HomePage
 * 说明：检索界面主页，主要由输入框和检索按钮组成
 *      当输入时按下回车或者点击按钮，发出 retrieveRequest 信号
 */

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
