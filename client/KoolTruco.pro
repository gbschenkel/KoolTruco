#-------------------------------------------------
#
# Project created by QtCreator 2011-10-22T23:53:52
#
#-------------------------------------------------

QT       += core gui network

TARGET = KoolTruco
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    clientthread.cpp \
    gametable.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    client.h \
    clientthread.h \
    gametable.h \
    dialog.h

FORMS    += mainwindow.ui \
         dialog.ui \
    gametable.ui \

RESOURCES += \
    kooltruco.qrc







unix:!symbian:!maemo5:isEmpty(MEEGO_VERSION_MAJOR) {
    target.path = /opt/KoolTruco/bin
    INSTALLS += target
}








