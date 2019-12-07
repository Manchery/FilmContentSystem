#ifndef RETRIEVEPAGE_H
#define RETRIEVEPAGE_H

#include <QWidget>

namespace Ui {
class RetrievePage;
}

class RetrievePage : public QWidget
{
    Q_OBJECT

public:
    explicit RetrievePage(QWidget *parent = nullptr);
    ~RetrievePage();

private:
    Ui::RetrievePage *ui;
};

#endif // RETRIEVEPAGE_H
