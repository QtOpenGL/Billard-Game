#
# Configuration pour les includepath et la lib billard
#

! include( ../test_common.pri ) {
    error( "Impossible de trouver le fichier commun test_common.pri" )
}

#
# Sources du subdir courant
#

CLASS_NAME = trou
SOURCES += $$ROOT/obstacle_immobile/$$CLASS_NAME/test_$${CLASS_NAME}.cpp
