TEMPLATE = app

QT += qml quick widgets network svg

SOURCES += main.cpp \
    cncontroller.cpp \
    cncontpetition.cpp \
    petitionlist.cpp \
    cnfileentry.cpp \
    cnfilemodel.cpp \
    petitionplay.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    cncontroller.h \
    cncontpetition.h \
    petitionlist.h \
    cnfileentry.h \
    cnfilemodel.h \
    petitionplay.h
