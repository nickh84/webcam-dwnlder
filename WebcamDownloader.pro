#-------------------------------------------------
#
# Project created by QtCreator 2012-08-15T23:18:31
#
#-------------------------------------------------

QT       += core gui network

TARGET = WebcamDownloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    getcam.cpp \
    newdialog.cpp \
    camtab.cpp

HEADERS  += mainwindow.h \
    config.h \
    getcam.h \
    newdialog.h \
    camtab.h

CONFIG +=

RESOURCES += \
    camrc.qrc
