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
    main.cpp \

