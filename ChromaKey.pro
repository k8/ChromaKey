# -------------------------------------------------
# Project created by QtCreator 2010-11-04T23:00:36
# -------------------------------------------------
TARGET = ChromaKey
TEMPLATE = app
SOURCES += main.cpp \
    mainwidget.cpp \
    moviethread.cpp \
    image.cpp \
    movielabel.cpp
HEADERS += mainwidget.h \
    moviethread.h \
    image.h \
    movielabel.h
FORMS += mainwidget.ui
CONFIG += link_pkgconfig
PKGCONFIG += --cflags \
    opencv \
    --libs \
    opencv
