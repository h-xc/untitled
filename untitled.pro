#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T22:20:25
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
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

CONFIG += c++11

SOURCES += \
        Public/ModelView/datastore.cpp \
        Public/ModelView/item_delegate.cpp \
        Public/ModelView/memitemmapper.cpp \
        Public/ModelView/mydatawidgetmapper.cpp \
        Public/ModelView/mydoublevalidator.cpp \
        Public/ModelView/mylineedit.cpp \
        Public/ModelView/settingpanel.cpp \
        Public/ModelView/widgetmappermodel.cpp \
        main.cpp \
        mainwindow.cpp \
        qqSetUp/qqsetup.cpp

HEADERS += \
        Public/ModelView/datastore.h \
        Public/ModelView/item_delegate.h \
        Public/ModelView/memitemmapper.h \
        Public/ModelView/mydatawidgetmapper.h \
        Public/ModelView/mydoublevalidator.h \
        Public/ModelView/mylineedit.h \
        Public/ModelView/settingpanel.h \
        Public/ModelView/widgetmappermodel.h \
        mainwindow.h \
        qqSetUp/qqsetup.h

FORMS += \
        mainwindow.ui \
        qqSetUp/qqsetup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc

DISTFILES +=
