TEMPLATE = app
TARGET = poker
QT = core gui
greaterThan (QT_MAJOR_VERSION, 4): QT += widgets
#CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11 -pthread

LIBS += -pthread \
    -L/home/gerard/CPP/pokergame/pokerengine/libs -lpokerengine

INCLUDEPATH = /home/gerard/CPP/pokergame/pokerengine/include

SOURCES += \
    main.cpp \
    Window.cpp \
    PokerTable.cpp \
    pixmaps.cpp \
    PlayerView.cpp \
    GameThread.cpp \
    ClickableLabel.cpp

HEADERS += \
    Window.h \
    PokerTable.h \
    pixmaps.h \
    PlayerView.h \
    GameThread.h \
    ClickableLabel.h

RESOURCES += \
    images.qrc

