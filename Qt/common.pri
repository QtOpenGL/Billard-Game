######################################################################
#
# Fichier configuration pour les subdirs
#
######################################################################

# Nous avons besoin de c++11 et autre options de compilation

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -O3
QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall

CONFIG += warn_off
CONFIG += c++11


# Nous avons besoin de la bibliotheque libBillard qui représente le moteur physique

LIBS += -L../Billard -lBillard
PRE_TARGETDEPS += ../Billard/libBillard.a

isEmpty(ROOT){
    ROOT = ../../Billard
}

# Nous avons besoin des chemins suivants pour inclure les headers du billard
# Nous n'incluons que les headers dont les controler et les gui ont besoin
# Il s'agit principalement des headers des objets (Boule, Paroi, etc.)

INCLUDEPATH += $$ROOT/boule   \
           $$ROOT/objet       \
           $$ROOT/vecteur     \
           $$ROOT/integrateur \
           $$ROOT/obstacle_immobile/brique \
           $$ROOT/obstacle_immobile/paroi  \
           $$ROOT/obstacle_immobile/sol    \
           $$ROOT/obstacle_immobile/trou   \
           $$ROOT/obstacle_immobile        \
           $$ROOT/viewer            \
           $$ROOT/viewer/text_viewer\
           $$ROOT/systeme           \
           $$ROOT/libraries/        \
           $$ROOT/libraries/multiple_dispatch/redistd-integer_seq \

# Les headers utilisés par les subdirs sont ceux des objets a dessiner

HEADERS += $$ROOT/objet/objet.h \
           $$ROOT/objet/coefficients_physiques.h \
           $$ROOT/vecteur/vecteur.h \
           $$ROOT/boule/boule.h \
           $$ROOT/boule/boule_de_couleur.h \
           $$ROOT/obstacle_immobile/brique/brique.h \
           $$ROOT/obstacle_immobile/paroi/paroi.h \
           $$ROOT/obstacle_immobile/sol/sol.h \
           $$ROOT/obstacle_immobile/trou/trou.h \
           $$ROOT/viewer/viewer.h \
           $$ROOT/viewer/object_viewer.h \
           $$ROOT/systeme/billard.h \
           $$ROOT/systeme/billard_builder.h
