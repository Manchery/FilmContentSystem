#ifndef FILMPAGE_H
#define FILMPAGE_H

#include "FilmContentSystemApplication.h"
#include "CharStringLink.h"
#include <QWidget>
#include <QHBoxLayout>

/* FilmPage
 * 说明：电影信息界面，展示电影海报、信息、推荐电影
 *      从检索界面进入时，高亮显示检索关键词
 *      点击可跳转至推荐电影
 */

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
    // 修改电影id，可实现电影的跳转，并发出 filmChanged 信号
    void setId(int id);
    // 高亮显示检索关键词
    void setHighlight(const CharStringLink& keywords);

private:
    Ui::FilmPage *ui;
    const FilmContentSystemApplication* app;
    int id; // 电影id
    QHBoxLayout *recommLayout;  // 推荐列表的layout
};

// 将 FilmInfo 类型转换成 html 格式的文本
extern QString info2String(const FilmInfo& info);

#endif // FILMPAGE_H
