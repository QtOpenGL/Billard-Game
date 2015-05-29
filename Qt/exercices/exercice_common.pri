######################################################################
#
# Fichier configuration pour les subdirs de test
#
######################################################################

# Nous avons besoin de c++11 et autre options de compilation

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
WARNINGS += -Wall
QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall
CONFIG += warn_off

# Nous avons besoin de la bibliotheque libBillard qui représente le moteur physique

isEmpty(ROOT){
    ROOT = ../../../Billard
}

LIBS += -L../../Billard -lBillard
PRE_TARGETDEPS += ../../Billard/libBillard.a

# Nous avons besoin des chemins suivants pour inclure les headers du billard
# Il y en a plus que dans le fichier common.pri parce que les tests font parti du billard

INCLUDEPATH += . \
               $$ROOT/boule \
               $$ROOT/vecteur \
               $$ROOT/objet \
               $$ROOT/integrateur \
               $$ROOT/libraries \
               $$ROOT/libraries/multiple_dispatch \
               $$ROOT/libraries/multiple_dispatch/redistd-integer_seq \
               $$ROOT/libraries/string_matcher \
               $$ROOT/libraries/cpp_sscanf \
               $$ROOT/collision \
               $$ROOT/obstacle_immobile \
               $$ROOT/obstacle_immobile/sol \
               $$ROOT/obstacle_immobile/paroi \
               $$ROOT/obstacle_immobile/trou \
               $$ROOT/libraries/pave \
               $$ROOT/obstacle_immobile/brique \
               $$ROOT/systeme \
               $$ROOT/viewer \
               $$ROOT/viewer/text_viewer \
               $$ROOT/libraries/cpp_sscanf \
               $$ROOT/libraries/3rd_party

# Les headers utilisés par les subdirs sont ceux des objets a dessiner

HEADERS2 = $$ROOT/boule/boule.h \
           $$ROOT/trou/trou.h \
           $$ROOT/boule/boule_de_couleur.h \
           $$ROOT/objet/objet.h \
           $$ROOT/objet/coefficients_physiques.h \
           $$ROOT/vecteur/vecteur.h \
           $$ROOT/obstacle_immobile/brique/brique.h \
           $$ROOT/obstacle_immobile/paroi/paroi.h \
           $$ROOT/obstacle_immobile/sol/sol.h \
           $$ROOT/viewer/viewer.h \
           $$ROOT/viewer/object_viewer.h \
           $$ROOT/systeme/billard.h \
           $$ROOT/systeme/billard_builder.h
