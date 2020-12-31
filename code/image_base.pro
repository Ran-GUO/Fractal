#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:40:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = triangle
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    mandelbrotimage.cpp \
    splineimage.cpp


HEADERS  += mainwindow.h \
    elec4_util.h \
    mandelbrotimage.h \
    splineimage.h

#QMAKE_CXXFLAGS += -std=c++14 -mavx

QMAKE_CXXFLAGS_DEBUG =-g
QMAKE_CXXFLAGS_RELEASE =-O3

QMAKE_CXXFLAGS += -std=c++14 -Wno-attributes

INCLUDEPATH +=D:\MSYS2\msys2\home\admin\include\eigen
