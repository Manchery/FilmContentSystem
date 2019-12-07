#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>
#include <locale>

int main(int argc, char *argv[])
{
    std::locale::global(std::locale("chs"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
