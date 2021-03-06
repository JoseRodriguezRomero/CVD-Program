#-------------------------------------------------
#
# Project created by QtCreator 2017-07-18T12:12:55
#
#-------------------------------------------------

QT       += core gui serialport charts

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = CVD-Program
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += manualcontrolpage.cpp \
    statusstring.cpp \
    recipespage.cpp \
    mainwindow.cpp \
    main.cpp \
    serial/eurothermserialclass.cpp \
    serialsettingswindow.cpp \
    logspage.cpp \
    serial/pfeifferserialclass.cpp \
    serial/baseserialclass.cpp \
    serial/mksserialclass.cpp

HEADERS  += manualcontrolpage.h \
    statusstring.h \
    recipespage.h \
    mainwindow.h \
    serial/eurothermserialclass.h \
    serialsettingswindow.h \
    logspage.h \
    serial/pfeifferserialclass.h \
    serial/baseserialclass.h \
    serial/mksserialclass.h
