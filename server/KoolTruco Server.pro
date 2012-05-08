QT += core gui network

TARGET = KoolTruco_Server
TEMPLATE = app

SOURCES = player.cpp \
    card.cpp \
    deck.cpp \
    dialog.cpp \
    main.cpp \
    server.cpp \
    thread.cpp

HEADERS = player.h \
    card.h \
    deck.h \
    dialog.h \
    server.h \
    thread.h


