######################################################################
# Automatically generated by qmake (2.01a) ?? 11? 1 13:46:56 2016
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . Help trans
INCLUDEPATH += . Help

# Input
HEADERS += Config.h \
           Defpam.h \
           em335x_drivers.h \
           KEY.h \
           keypads.h \
           Testcom2.h \
           thread1.h \
           Volseq.h \
           widget.h \
           Help/helpdialog.h
FORMS += keypads.ui widget.ui Help/helpdialog.ui
SOURCES += KEY.cpp \
           keypads.cpp \
           main.cpp \
           Testcom2.cpp \
           thread1.cpp \
           Volseq.cpp \
           widget.cpp \
           Help/helpdialog.cpp
RESOURCES += myresource.qrc
TRANSLATIONS += trans/en.ts trans/zh.ts
