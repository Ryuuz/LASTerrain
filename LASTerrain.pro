#-------------------------------------------------
#
# Project created by QtCreator 2018-09-11T22:41:36
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LASTerrain
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    axis.cpp \
    boundingvolume.cpp \
    camera.cpp \
    colormaterial.cpp \
    light.cpp \
    lightmaterial.cpp \
    material.cpp \
    objectinstance.cpp \
    physicsobject.cpp \
    pointcloud.cpp \
    renderwindow.cpp \
    sceneobject.cpp \
    shader.cpp \
    sphere.cpp \
    transform.cpp \
    vertex.cpp \
    byteconverter.cpp \
    collision.cpp

HEADERS += \
        mainwindow.h \
    axis.h \
    boundingvolume.h \
    camera.h \
    colormaterial.h \
    constants.h \
    light.h \
    lightmaterial.h \
    material.h \
    objectinstance.h \
    physicsobject.h \
    pointcloud.h \
    renderwindow.h \
    sceneobject.h \
    shader.h \
    sphere.h \
    transform.h \
    vertex.h \
    byteconverter.h \
    lasdefinitions.h \
    collision.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fragmentshader.frag \
    lightshader.frag \
    lightshader.vert \
    vertexshader.vert
