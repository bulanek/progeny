#-------------------------------------------------
#
# Project created by QtCreator 2013-09-13T08:54:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = progeny
TEMPLATE = app

LIBS+= -lgsl -lgslcblas
QT+=sql
QT+=xml


SOURCES += main.cpp\
        mainwindow.cpp \
    progenyMatrix.cc \
    datahandle.cpp \
    tools.cc \
    sqlconnection.cpp \
    sqlhandle.cpp \
    measurementdata.cpp \
    progeny_time_dependence.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    progenyMatrix.hh \
    datahandle.h \
    tools.h \
    sqlconnection.h \
    sqlhandle.h \
    measurementdata.h \
    progeny_time_dependence.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    sqlconnection.ui \
    measurementdata.ui \
    progeny_time_dependence.ui
