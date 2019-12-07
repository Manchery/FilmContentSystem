#include "homepage.h"
#include "ui_homepage.h"
#include <QLineEdit>
#include <QPixmap>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    QPixmap icon(":/icon/resource/icon.png");
    ui->IconLabel->setPixmap(QPixmap(icon).scaled(icon.size()*0.5));
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
