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
    newdialog.cpp \
    camtab.cpp \
    camviewport.cpp

HEADERS  += mainwindow.h \
    config.h \
    newdialog.h \
    camtab.h \
    camviewport.h

CONFIG +=

RESOURCES += \
    camrc.qrc
