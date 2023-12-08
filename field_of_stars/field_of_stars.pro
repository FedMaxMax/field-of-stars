TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG +=c++11

LIBS += -L"C:\Users\Maxim Jr\moq katalog\C++\Uses\SFML-2.3.2\lib"

CONFIG(release, debug|release):
LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

CONFIG(debug, debug|release):
LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "C:\Users\Maxim Jr\moq katalog\C++\Uses\SFML-2.3.2\include"

SOURCES += main.cpp \
    player.cpp \
    entity.cpp \
    enemy.cpp \
    bullet.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    player.h \
    enemy.h \
    bullet.h \
    entity.h \
    global.h

