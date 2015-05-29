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

INCLUDEPATH +=  \
    billard/    \
    vue_opengl/ \

SOURCES += \
    main.cpp \
    glwidget.cc \
    vue_opengl/vue_opengl.cc \
    vue_opengl/glsphere.cc \
    billard/opengl_viewer.cpp \
    billard/boule_de_couleur.cpp \
    billard/telecommande_boule.cpp \
    billard/boule_invincible.cpp

HEADERS += \
    glwidget.h \
    vue_opengl/vertex_shader.h \
    vue_opengl/vue_opengl.h \
    vue_opengl/glsphere.h \
    vue_opengl/polygon.h \
    billard/billard_to_qt.h \
    billard/opengl_viewer.h \
    billard/boule_de_couleur_couleur.h \
    billard/telecommande_boule.h \
    billard/boule_invincible.h

RESOURCES += \
    vue_opengl/shaders.qrc
