#-------------------------------------------------
#
# Project created by QtCreator 2010-11-22T14:32:47
#
#-------------------------------------------------

QT       += core gui

TARGET = DebugVisualisator
TEMPLATE = app


SOURCES += src/Debug/Visualisator/main.cpp              \
           src/Debug/Visualisator/mainwindow.cpp        \

HEADERS  += include/Debug/Visualisator/mainwindow.h     \
            include/Debug/Visualisator/ui_mainwindow.h  \

FORMS    += include/Debug/Visualisator/mainwindow.ui    \

LIBS += -lpthread -lfontconfig -lclan22App -lclan22Core -lclan22Network

OBJECTS_DIR = build/Visualisator/Objects-files
MOC_DIR = build/Visualisator/Moc-files
RCC_DIR = build/Visualisator/Rcc-files

# Location of the binary file
DESTDIR = bin

#MAKEFILE = build/Visualisator/Makefile
