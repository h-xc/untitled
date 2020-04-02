#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T23:11:38
#
#-------------------------------------------------

QT       += gui-private widgets

CONFIG += plugin

TARGET = Qt5SoftKeyboard
TEMPLATE = lib

DEFINES += VIRTUALKEYBOARD_LIBRARY


SOURCES += \
    inputcontext.cpp \
    platforminputcontextplugin.cpp \
    googlepinyin.cpp \
    keyboardform.cpp \
    keyeventdispatcher.cpp


HEADERS +=\
    pinyinime.h \
    inputcontext.h \
    platforminputcontextplugin.h \
    virtualkeyboard_global.h \
    googlepinyin.h \
    keyboardform.h \
    keyeventdispatcher.h

RESOURCES += \
    res.qrc

CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../xdebug/platforminputcontexts
}else{
    DESTDIR =$$PWD/../xrelease/platforminputcontexts
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyind
else:unix: LIBS += -L$$PWD/pinyin/ -lgooglepinyin

INCLUDEPATH += $$PWD/pinyin
DEPENDPATH += $$PWD/pinyin
