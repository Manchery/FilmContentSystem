#ifndef RETRIEVEPAGE_H
#define RETRIEVEPAGE_H

#include "CharStringLink.h"
#include "FilmContentSystemApplication.h"
#include <QWidget>
#include <QVBoxLayout>
#include <algorithm>

/* RetrievePage
 * 说明：检索结果页面，显示检索关键字和检索结果（突出显示关键字）
 *      在输入框修改可再次检索
 *      点击检索结果可跳转至电影页面
 */

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
    // 检索，_keywords 为关键字
    void retrieve(const CharStringLink& _keywords);
    // 修改 keywordsEdit 中的文本
    void setText(QString text);

private slots:
    // 发出 keywordsChanged 信号
    void on_searchButton_clicked();
    void on_keywordsEdit_returnPressed();

private:
    Ui::RetrievePage *ui;
    FilmContentSystemApplication* app;
    CharStringLink keywords;    // 分词后的检索关键字
    QVBoxLayout *resultLayout;  // 检索结果列表的 layout

    // 返回一条检索结果的 widget
    QWidget *retrieResultItem(std::pair<int, std::pair<int, int>> res);
    // 返回一条检索结果的摘要，由关键词出现的片段组成，突出显示关键词
    QString abstract(const QString &text, const CharStringLink &keywords);
};

#endif // RETRIEVEPAGE_H
