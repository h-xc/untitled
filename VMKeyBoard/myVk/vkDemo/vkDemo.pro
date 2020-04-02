#-------------------------------------------------
#
# Project created by QtCreator 2020-04-01T14:59:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vkDemo
TEMPLATE = app

CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../xdebug
}else{
    DESTDIR =$$PWD/../xrelease
}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
