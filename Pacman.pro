#-------------------------------------------------
#
# Project created by QtCreator 2017-05-07T13:10:37
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pacman
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        pacman.cpp \
    gra.cpp \
    moje_klasy.cpp \
    debug/moc_gra.cpp \
    debug/moc_pacman.cpp \
    gra.cpp \
    main.cpp \
    moje_klasy.cpp \
    pacman.cpp

HEADERS  += pacman.h \
    gra.h \
    moje_klasy.h \
    debug/moc_predefs.h \
    gra.h \
    moje_klasy.h \
    pacman.h \
    ui_gra.h \
    ui_pacman.h

FORMS    += pacman.ui \
    gra.ui

DISTFILES += \
    debug/maps/mega.txt \
    debug/maps/plansza.txt \
    ../../Users/Łukasz/Box Sync/Cpp/build-Pacman-Desktop_Qt_5_8_0_MinGW_32bit-Debug/mega.docx \
    ../../Users/Łukasz/Box Sync/Cpp/build-Pacman-Desktop_Qt_5_8_0_MinGW_32bit-Debug/plansza.docx \
    ../../Users/Łukasz/Box Sync/Cpp/build-Pacman-Desktop_Qt_5_8_0_MinGW_32bit-Debug/plansza.txt \
    mapsCreate/mega.docx \
    mapsCreate/plansza.docx \
    mapsCreate/plansza.txt
