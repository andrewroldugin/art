DEFINES += __WORDSIZE=32
QMAKE_CXXFLAGS += --std=c++0x

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lsodium
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lsodium
else:unix: LIBS += -L$$PWD/ -lsodium

LIBS += -lpthread

INCLUDEPATH += $$PWD/ $$PWD/../boost/
#DEPENDPATH += $$PWD/

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/libsodium.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/libsodium.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/sodium.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/sodium.lib
#else:unix: PRE_TARGETDEPS += $$PWD/libsodium.a
