#-------------------------------------------------
#
# Project created by QtCreator 2020-08-07T16:57:19
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gxde-remote-desktop-accessor
TEMPLATE = app

#include(vnc/qvncclientwidget.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    qvncclientwidget.cpp

HEADERS += \
    mainwindow.h \
    qvncclientwidget.h

VERSION = 1.0
BINDIR  = /opt/apps/spark-simple-remote-desktop-accessor/files/bin
target.path = $$BINDIR

icon.files = spark-simple-remote-desktop-accessor.svg
icon.path = /opt/apps/spark-simple-remote-desktop-accessor/entries/icons/hicolor/scalable

desktop.files = spark-simple-remote-desktop-accessor.desktop
desktop.path = /opt/apps/spark-simple-remote-desktop-accessor/entries/applications/

INSTALLS += target icon desktop

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin


FORMS +=
