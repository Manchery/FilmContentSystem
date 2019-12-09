#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FilmContentSystemApplication.h"
#include "homepage.h"
#include <QMainWindow>

/* MainWindow
 * 说明：主界面类
 *      管理由HomePage、RetrievePage、FilmPage 组成的多个标签页
 *      如果主页检索的关键字与电影名完美匹配则直接跳转到电影页面
 *      否则跳转到检索结果页面
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // 检索，keywords_raw 是未分词的关键字
    void retrieve(QString keywords_raw);

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs;
    FilmContentSystemApplication *app;

    // 新建一个 FilmPage 类型的标签页
    void newFilmPage(int id, const CharStringLink &keywords);
    // 新建一个 RetrievePage 类型的标签页
    void newRetrievePage(QString keywords_raw, const CharStringLink &keywords);
};

#endif // MAINWINDOW_H
