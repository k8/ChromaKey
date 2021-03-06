# -------------------------------------------------
# Project created by QtCreator 2010-11-04T23:00:36
# -------------------------------------------------
TARGET = ChromaKey
TEMPLATE = app
SOURCES += main.cpp \
    mainwidget.cpp \
    keyingthread.cpp \
    imagesprocessor.cpp \
    movielabel.cpp \
    imagessupplier.cpp \
    filesavingdialog.cpp \
    keyingparameters.cpp \
    pausablethread.cpp \
    savingthread.cpp \
    realtimethread.cpp \
    image.cpp \
    movie.cpp \
    imagefactory.cpp \
    matte.cpp \
    keyer.cpp \
    hsvkeyer.cpp \
    differencekeyer.cpp \
    keyerfactory.cpp \
    movieslider.cpp
HEADERS += mainwidget.h \
    keyingthread.h \
    imagesprocessor.h \
    movielabel.h \
    imagessupplier.h \
    filesavingdialog.h \
    keyingparameters.h \
    pausablethread.h \
    savingthread.h \
    realtimethread.h \
    image.h \
    movie.h \
    imagefactory.h \
    matte.h \
    keyer.h \
    hsvkeyer.h \
    differencekeyer.h \
    keyerfactory.h \
    movieslider.h
FORMS += mainwidget.ui \
    filesavingdialog.ui
CONFIG += link_pkgconfig
PKGCONFIG += --cflags \
    opencv \
    --libs \
    opencv
