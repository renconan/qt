######################################################################
# Automatically generated by qmake (2.01a) ?? 11? 11 11:48:48 2016
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . Help trans
INCLUDEPATH += . Help

# Input
HEADERS += Config.h \
           Curseq.h \
           Defpam.h \
           em335x_drivers.h \
           KEY.h \
           keypads.h \
           Testcom2.h \
           thread1.h \
           widget.h \
           Help/helpdialog.h
FORMS += keypads.ui widget.ui Help/helpdialog.ui
SOURCES += Curseq.cpp \
           KEY.cpp \
           keypads.cpp \
           main.cpp \
           Testcom2.cpp \
           thread1.cpp \
           widget.cpp \
           Help/helpdialog.cpp
RESOURCES += myresource.qrc
TRANSLATIONS += trans/en.ts trans/zh.ts
