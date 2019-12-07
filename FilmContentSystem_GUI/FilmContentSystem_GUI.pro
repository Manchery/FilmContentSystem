#-------------------------------------------------
#
# Project created by QtCreator 2019-12-07T09:03:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilmContentSystem_GUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
    ../FilmContentSystem/CharString.cpp \
    ../FilmContentSystem/CharStringLink.cpp \
    ../FilmContentSystem/common.cpp \
    ../FilmContentSystem/DocumentList.cpp \
    ../FilmContentSystem/FileReader.cpp \
    ../FilmContentSystem/FilmContentSystemApplication.cpp \
    ../FilmContentSystem/FilmInfo.cpp \
    ../FilmContentSystem/HtmlParser.cpp \
    ../FilmContentSystem/HtmlTag.cpp \
    ../FilmContentSystem/WordSegmentor.cpp \
    clickablelabel.cpp \
    common_gui.cpp \
    filmpage.cpp \
    homepage.cpp \
    main.cpp \
    mainwindow.cpp \
    retrievepage.cpp

HEADERS += \
    ../FilmContentSystem/*.hpp \
    ../FilmContentSystem/*.h \
    clickablelabel.h \
    common_gui.h \
    filmpage.h \
    homepage.h \
    mainwindow.h \
    retrievepage.h \


FORMS += \
    filmpage.ui \
    homepage.ui \
    mainwindow.ui \
    retrievepage.ui

INCLUDEPATH += ../FilmContentSystem/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target