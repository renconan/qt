######################################################################
# Automatically generated by qmake (2.01a) ?? 7? 7 12:46:33 2017
######################################################################
LIBS += -L./ -lBroadCastingUpdClient
TEMPLATE = app
TARGET = 
DEPENDPATH += . Import trans
INCLUDEPATH += . Import

# Input
HEADERS += BroadCastingUpdClient.h \
           Config.h \
           Defpam.h \
           em335x_drivers.h \
           exportreport.h \
           Gpio.h \
           KEY.h \
           Nodelinkmanage.h \
           Testcom2.h \
           thread1.h \
           Uartset.h \
           widget.h \
           Import/import.h \
           Import/importthread.h
FORMS += exportreport.ui widget.ui Import/import.ui
SOURCES += exportreport.cpp \
           Gpio.cpp \
           KEY.cpp \
           main.cpp \
           Nodelinkmanage.cpp \
           Testcom2.cpp \
           thread1.cpp \
           Uartset.cpp \
           widget.cpp \
           Import/import.cpp \
           Import/importthread.cpp
RESOURCES += myresouce.qrc
TRANSLATIONS += trans/en.ts trans/zh.ts