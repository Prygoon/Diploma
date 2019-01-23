#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T11:09:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyWork
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    inputwindow.cpp \
    locomotive.cpp \
    tracksection.cpp \
    railcar.cpp \
    database.cpp \
    project.cpp \
    train.cpp \
    locomotivedbwindow.cpp \
    railcardbwindow.cpp

HEADERS += \
        mainwindow.h \
    inputwindow.h \
    locomotive.h \
    tracksection.h \
    railcar.h \
    database.h \
    project.h \
    train.h \
    locomotivedbwindow.h \
    railcardbwindow.h

FORMS += \
        mainwindow.ui \
    inputwindow.ui \
    locomotivedbwindow.ui \
    railcardbwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
