#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T10:18:57
#
#-------------------------------------------------

QT       += core gui-private gui widgets dbus

TARGET = vkimplatforminputcontextplugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/platforminputcontexts

SOURCES += vkimplatforminputcontextplugin.cpp \
    vkimplatforminputcontext.cpp

HEADERS += vkimplatforminputcontextplugin.h \
    vkimplatforminputcontext.h
DISTFILES += \
    vkim.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += ../../include
