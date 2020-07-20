#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T16:04:09
#
#-------------------------------------------------

QT       += core gui opengl
QT       += widgets

TARGET = VisualDataGPU
TEMPLATE = app

SOURCES +=\
    Camera.cpp \
    DataAnimation.cpp \
    Light.cpp \
    Material.cpp \
    MainWindow.cpp \
    Main.cpp \
    GLWidget.cpp \
    TG/Translation.cpp \
    TG/scale.cpp \
    Toonmaterial.cpp \
    dialog.cpp \
    library/Common.cpp \
    objects/CubBackground.cpp \
    objects/cara.cpp \
    objects/Object.cpp \
    objects/groundplane.cpp \
    objects/plane.cpp \
    readers/datareader.cpp \
    readers/scenereader.cpp \
    scenes/Scene.cpp \
    TG/TG.cpp \
    animations/Animation.cpp \
    Builder.cpp \
    colormaps/ColorMapStatic.cpp

HEADERS  += \
    DataAnimation.h \
    TG/Translation.h \
    TG/scale.h \
    Toonmaterial.h \
    dialog.h \
    library/vec.h \
    library/mat.h \
    library/Common.h \
    Camera.h \
    Light.h \
    Material.h \
    MainWindow.h \
    objects/CubBackground.h \
    objects/cara.h \
    objects/Object.h \
    objects/groundplane.h \
    objects/plane.h \
    readers/datareader.h \
    readers/scenereader.h \
    scenes/Scene.h \
    GLWidget.h \
    TG/TG.h \
    animations/Animation.h \
    Builder.h \
    colormaps/ColorMap.h \
    colormaps/ColorMapStatic.h

FORMS    += mainwindow.ui \
    dialog.ui

OTHER_FILES += \
    vshader1.glsl \
    fshader1.glsl

RESOURCES += resources.qrc

INCLUDEPATH+=/opt/X11/include

DISTFILES += \
    resources/textures/earth1.png \
    resources/textures/earth3.png
