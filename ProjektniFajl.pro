QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp \
    glavnastrana.cpp \
    dugme.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    glavnastrana.h \
    dugme.h

RESOURCES += \
    res.qrc

