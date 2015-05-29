#
# Configuration pour les includepath et la lib billard
#

! include( ../test_common.pri ) {
    error( "Impossible de trouver le fichier commun test_common.pri" )
}

#
# Sources du subdir courant
#

SOURCES += $$ROOT/vecteur/test_vecteur.cpp
