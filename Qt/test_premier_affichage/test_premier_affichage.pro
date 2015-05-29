QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app_billard_opengl

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
    glsphere.cc \
    premier_affichage.cc

HEADERS += \
    glwidget.h \
    vertex_shader.h \
    vue_opengl.h \
    glsphere.h \
    billard_to_qt.h \
    polygon.h
    
RESOURCES += \
    resource.qrc
