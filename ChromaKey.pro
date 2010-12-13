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
    imagessupplier.cpp \
    filesavingdialog.cpp \
    keyingparameters.cpp \
    pausablethread.cpp \
    savingthread.cpp \
    realtimethread.cpp
HEADERS += mainwidget.h \
    keyingthread.h \
    image.h \
    movielabel.h \
    imagessupplier.h \
    filesavingdialog.h \
    keyingparameters.h \
    pausablethread.h \
    savingthread.h \
    realtimethread.h
FORMS += mainwidget.ui \
    filesavingdialog.ui
CONFIG += link_pkgconfig
PKGCONFIG += --cflags \
    opencv \
    --libs \
    opencv
