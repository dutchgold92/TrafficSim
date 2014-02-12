#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T11:13:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficSim2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vehicle.cpp \
    car.cpp \
    cell.cpp \
    road.cpp \
    network.cpp \
    junction.cpp \
    networkupdater.cpp \
    graphicscellitem.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    vehicle.h \
    car.h \
    cell.h \
    road.h \
    network.h \
    junction.h \
    networkupdater.h \
    graphicscellitem.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    plotter.ui
