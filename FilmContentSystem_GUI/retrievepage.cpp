#include "retrievepage.h"
#include "ui_retrievepage.h"

RetrievePage::RetrievePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RetrievePage)
{
    ui->setupUi(this);
}

RetrievePage::~RetrievePage()
{
    delete ui;
}
