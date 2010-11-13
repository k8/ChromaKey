# -------------------------------------------------
# Project created by QtCreator 2010-11-04T23:00:36
# -------------------------------------------------
TARGET = ChromaKey
TEMPLATE = app
SOURCES += main.cpp \
    mainwidget.cpp \
    keyingthread.cpp \
    image.cpp \
    movielabel.cpp \
    imagessupplier.cpp
HEADERS += mainwidget.h \
    keyingthread.h \
    image.h \
    movielabel.h \
    imagessupplier.h
FORMS += mainwidget.ui
CONFIG += link_pkgconfig
PKGCONFIG += --cflags \
    opencv \
    --libs \
    opencv
