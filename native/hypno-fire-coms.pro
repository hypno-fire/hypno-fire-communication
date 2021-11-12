
CONFIG += c++11
CONFIG += plugin
CONFIG -= qt
CONFIG+=link_pkgconfig

TARGET = HypnoFireComs
TEMPLATE = lib

DEFINES += QMLNET_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../messages/nanopdb/
INCLUDEPATH += $$PWD/../src/

HEADERS += \
    client.h \
    $$PWD/../src/messages.pb.h \
    $$PWD/../messages/nanopdb/pb.h \
    $$PWD/../messages/nanopdb/pb_common.h \
    $$PWD/../messages/nanopdb/pb_encode.h \
    $$PWD/Encoding/COBS.h

SOURCES += \
    $$PWD/../src/messages.pb.c \
    $$PWD/../messages/nanopdb/pb_common.c \
    $$PWD/../messages/nanopdb/pb_encode.c \
    client.cpp

target.path = $$(PREFIX)/
INSTALLS += target


PKGCONFIG += libserialport
