#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T10:19:55
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keyboard
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    inputservice.cpp

HEADERS  += dialog.h \
    inputservice.h

FORMS    +=

DISTFILES +=

RESOURCES += res.qrc
