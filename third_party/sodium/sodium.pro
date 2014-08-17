#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T00:37:40
#
#-------------------------------------------------

QT       -= core gui

TARGET = sodium
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    sodium/light_ptr.cpp \
    sodium/lock_pool.cpp \
    sodium/sodium.cpp \
    sodium/transaction.cpp

HEADERS += \
    sodium/count_set.h \
    sodium/light_ptr.h \
    sodium/lock_pool.h \
    sodium/sodium.h \
    sodium/transaction.h \
    sodium/unit.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

#QMAKE_CXXFLAGS += -D__WORDSIZE=32 -I"I:/dev/projects/third_party/boost" --std=c++0x

include(sodium.pri)

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lsodium
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lsodium
#else:unix: LIBS += -L$$OUT_PWD/ -lsodium

#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/release/libsodium.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/debug/libsodium.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/release/sodium.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/debug/sodium.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/libsodium.a
