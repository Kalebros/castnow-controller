#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T13:58:01
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = cnDaemon
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    cndaemon.cpp \
    logger.cpp \
    petitionattende.cpp \
    cacheentry.cpp \
    cnprocess.cpp

HEADERS += \
    cndaemon.h \
    logger.h \
    petitionattende.h \
    cacheentry.h \
    cnprocess.h
