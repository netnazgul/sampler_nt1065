#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T09:38:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Sampler_NT1065
TEMPLATE = app

include(app_ver.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    cy3device.cpp \
    dataprocessor.cpp \
    qcustomplot/qcustomplot.cpp \
    spectrumform.cpp \
    fft/complex.cpp \
    fft/fft.cpp

HEADERS  += mainwindow.h \
    cy3device.h \
    dataprocessor.h \
    qcustomplot/qcustomplot.h \
    spectrumform.h \
    fft/complex.h \
    fft/fft.h

FORMS    += mainwindow.ui \
    spectrumform.ui

win32-msvc2013 {
    HEADERS += vld/vld.h

    LIBS += $$PWD\cyapi\lib\x86\CyAPI.lib
 #   LIBS += /NODEFAULTLIB:LIBCMT
    LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\User32.lib"
    LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\setupapi.lib"

    LIBS += -L$$PWD/vld/ -vld
    INCLUDEPATH += $$PWD/vld
    DEPENDPATH += $$PWD/vld

    PRE_TARGETDEPS += $$PWD/vld/vld.lib

    INCLUDEPATH += "%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Include"
    QMAKE_CXX += /D_USING_V110_SDK71_

    # 32bit toolchain
    QMAKE_LFLAGS += /SUBSYSTEM:CONSOLE,5.01
    LIBS += -L"%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Lib"


}

win32-msvc2010 {
    HEADERS += vld/vld.h

    LIBS += $$PWD\cyapi\lib\x86\CyAPI.lib
#    LIBS += /NODEFAULTLIB:LIBCMT
    LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\User32.lib"
    LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\setupapi.lib"

    LIBS += -L$$PWD/vld/ -vld
    INCLUDEPATH += $$PWD/vld
    DEPENDPATH += $$PWD/vld

    PRE_TARGETDEPS += $$PWD/vld/vld.lib
}

unix {

    #LIBS += -L/usr/lib/i386-linux-gnu/ -lSerialPort
}
