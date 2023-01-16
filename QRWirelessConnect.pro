QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport

CONFIG += c++17
CONFIG += link_pkgconfig
# Don't forget to set PKG_CONFIG_PATH to :/xyz/share/pkgconfig/ (xyz is your PREFIX) in the build environment vars
PKGCONFIG += libqrencode

include(version.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dialog_about.cpp \
    src/dialog_scale.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/dialog_about.h \
    src/dialog_scale.h \
    src/mainwindow.h

FORMS += \
    src/dialog_about.ui \
    src/dialog_scale.ui \
    src/mainwindow.ui

# includes dir
QMAKE_INCDIR += $$PREFIX/include
QMAKE_INCDIR += src
INCLUDEPATH += $$PREFIX/include
INCLUDEPATH += src

# INSTALLATION:
target.path = $$PREFIX/lib64
header_files.files = $$HEADERS
header_files.path = $$PREFIX/include/$$TARGET
INSTALLS += target
INSTALLS += header_files

# PKGCONFIG
CONFIG += create_pc create_prl no_install_prl
QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/lib64/
QMAKE_PKGCONFIG_INCDIR = $$PREFIX/include/$$TARGET
QMAKE_PKGCONFIG_CFLAGS = -I$$PREFIX/include/
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
#$$PREFIX/share/

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
