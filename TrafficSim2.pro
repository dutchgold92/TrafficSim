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
    truck.cpp \
    cell.cpp \
    road.cpp \
    network.cpp \
    junction.cpp \
    networkupdater.cpp

HEADERS  += mainwindow.h \
    vehicle.h \
    car.h \
    truck.h \
    cell.h \
    road.h \
    network.h \
    junction.h \
    networkupdater.h

FORMS    += mainwindow.ui
