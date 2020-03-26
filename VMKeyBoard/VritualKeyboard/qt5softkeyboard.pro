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

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyin
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyind

#INCLUDEPATH += $$PWD/pinyin
#DEPENDPATH += $$PWD/pinyin

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



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pinyin/ -lgooglepinyind
else:unix: LIBS += -L$$PWD/pinyin/ -lgooglepinyin

INCLUDEPATH += $$PWD/pinyin
DEPENDPATH += $$PWD/pinyin

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pinyin/libgooglepinyin.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pinyin/libgooglepinyind.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/pinyin/googlepinyin.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/pinyin/googlepinyind.lib
else:unix: PRE_TARGETDEPS += $$PWD/pinyin/libgooglepinyin.a
