TEMPLATE = app
TARGET = generichid
CONFIG(debug,debug|release) {
DESTDIR = debug
} else {
DESTDIR = release
}
QT += xml svg
QTPLUGIN += qjpeg
CONFIG += precompile_header link_prl
DEFINES += QT_XML_LIB QT_SVG_LIB USING_PCH
INCLUDEPATH += ./generatedfiles \
    ./generatedfiles/$(DESTDIR) \
    ../include \
    ../utility \
    ../programmer \
    ../dfuprogrammer \
    ../libusb-0.1.12 \
    ../qtpropertybrowser-2.5-opensource/src \
    ../makeeeprom \
    . \
    ../usbhid \
    ../qtcommon

CONFIG(debug,debug|release) {
INCLUDEPATH += ./generatedfiles/debug
} else {
INCLUDEPATH += ./generatedfiles/release
}

CONFIG(debug,debug|release) {
    unix:LIBS += -L../qtcommon/debug -lqtcommon -L../usbhid/debug -lusbhid -L../makeeeprom/debug -lmakeeeprom -L../utility/debug -lutility -L../dfuprogrammer/debug -ldfuprogrammer  -L../programmer/debug -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a -L"../qtpropertybrowser-2.5-opensource/lib" -l:libQtSolutions_PropertyBrowser-2.5.a -lutility -lutility -lutility -L$(QTDIR)/plugins/imageformats -lqjpeg
} else {
    unix:LIBS += -L../qtcommon/release -lqtcommon -L../usbhid/release -lusbhid -L../makeeeprom/release -lmakeeeprom -L../utility/release -lutility -L../dfuprogrammer/release -ldfuprogrammer  -L../programmer/release -lprogrammer -L../libusb-0.1.12/.libs -l:libusb.a -ldfuprogrammer -lutility -lprogrammer -ldfuprogrammer -lutility -l:libusb.a -L"../qtpropertybrowser-2.5-opensource/lib" -l:libQtSolutions_PropertyBrowser-2.5.a -lutility -lutility -lutility -L$(QTDIR)/plugins/imageformats -lqjpeg
}


PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
CONFIG(debug,debug|release) {
    MOC_DIR += ./generatedfiles/debug
    OBJECTS_DIR += debug
} else {
    MOC_DIR += ./generatedfiles/release
    OBJECTS_DIR += release
}

UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(generichid.pri)

