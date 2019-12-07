#include "homepage.h"
#include "ui_homepage.h"
#include <QLineEdit>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::on_searchButton_clicked()
{
    emit retrieveRequest(ui->keywordEdit->text());
}

void HomePage::on_keywordEdit_returnPressed()
{
    emit retrieveRequest(ui->keywordEdit->text());
}
