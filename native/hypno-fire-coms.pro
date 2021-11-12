
CONFIG += c++11
CONFIG += plugin
CONFIG -= qt
CONFIG+=link_pkgconfig

TARGET = HypnoFireComs
TEMPLATE = lib

DEFINES += QMLNET_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../messages/nanopdb/
INCLUDEPATH += $$PWD/../messages/

HEADERS += \
    client.h \
    $$PWD/../messages/messages.pb.h \
    $$PWD/../messages/nanopdb/pb.h \
    $$PWD/../messages/nanopdb/pb-encode.h \
    test.h

SOURCES += \
    $$PWD/../messages/messages.pb.c \
    client.cpp \
    test.cpp

target.path = $$(PREFIX)/
INSTALLS += target


PKGCONFIG += libserialport
