QT             += opengl
QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_glwidget

#
# Configuration pour les includepath et la lib billard
#

! include( ../common.pri ) {
    error( "Impossible de trouver le fichier commun common.pri" )
}

#
# Sources du subdir courant
#

SOURCES += \
    glwidget.cc \
    vue_opengl.cc \
    test_glwidget.cc

HEADERS += \
    glwidget.h \
    vertex_shader.h \
    vue_opengl.h
    
RESOURCES += \
    resource.qrc
