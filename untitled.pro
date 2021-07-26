#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T22:20:25
#
#-------------------------------------------------

QT       +=quick qml core gui xml  network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app
DESTDIR =$$PWD/xdebug

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
        qqSetUp/qqsetup.cpp \
    LayoutTest/layouttest.cpp \
    Public/ModelView/xmldatastore.cpp \
    setTime/SetTime.cpp \
    setTime/VerticalScroll.cpp \
    sysSet/sysset.cpp \
    WinCMD/winCmd.cpp \
    Socket/socket.cpp \
    LayoutForm/layoutform.cpp \
    toolbar.cpp \
    StruOffse/struoffse.cpp

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
        qqSetUp/qqsetup.h \
    LayoutTest/layouttest.h \
    Public/ModelView/xmldatastore.h \
    setTime/SetTime.h \
    setTime/VerticalScroll.h \
    sysSet/sysset.h \
    WinCMD/winCmd.h \
    Socket/socket.h \
    LayoutForm/layoutform.h \
    toolbar.h \
    StruOffse/struoffse.h

FORMS += \
        mainwindow.ui \
        qqSetUp/qqsetup.ui \
    LayoutTest/layouttest.ui \
    setTime/SetTime.ui \
    sysSet/sysset.ui \
    WinCMD/winCmd.ui \
    Socket/socket.ui \
    toolbar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc

DISTFILES +=
