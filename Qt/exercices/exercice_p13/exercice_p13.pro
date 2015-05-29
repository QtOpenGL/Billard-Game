#
# Configuration pour les includepath et la lib billard
#

! include( ../exercice_common.pri ) {
    error( "Impossible de trouver le fichier commun exercice_common.pri" )
}

#
# Sources du subdir courant
#

SOURCES += ../../../exercices/exercice_p13.cpp
