######################################################################
# Fichier pour compiler le coeur du billard
######################################################################

TARGET = Billard
TEMPLATE = lib

CONFIG = staticlib
CONFIG += c++11
QMAKE_CXXFLAGS += -O3

ROOT = ../../Billard

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

# Input
HEADERS += $$ROOT/boule/boule.h \
           $$ROOT/boule/boule_de_couleur.h \
           $$ROOT/boule/boule_fct_archivees.h \
           $$ROOT/collision/collision.h \
           $$ROOT/collision/detection_collision.h \
           $$ROOT/collision/gestion_collision.h \
           $$ROOT/collision/calculateur_ecart.h \
           $$ROOT/integrateur/integrable.h \
           $$ROOT/integrateur/integrateur.h \
           $$ROOT/libraries/almost_equal.h \
           $$ROOT/libraries/bandit.h \
           $$ROOT/libraries/multiple_dispatch.h \
           $$ROOT/libraries/multiple_dispatch/redistd-integer_seq/integer_seq.h \
           $$ROOT/libraries/raii.h \
           $$ROOT/libraries/trinome_second_degre.h \
           $$ROOT/objet/objet.h \
           $$ROOT/objet/objet_mobile.h \
           $$ROOT/objet/objet_visiteur.h \
           $$ROOT/obstacle_immobile/obstacle_immobile.h \
           $$ROOT/obstacle_immobile/trou/trou.h \
           $$ROOT/systeme/billard.h \
           $$ROOT/systeme/billard_general.h \
           $$ROOT/systeme/systeme.h \
           $$ROOT/vecteur/vecteur.h \
           $$ROOT/viewer/object_viewer.h \
           $$ROOT/viewer/viewer.h \
           $$ROOT/libraries/cpp_sscanf/converter_trait.h \
           $$ROOT/libraries/cpp_sscanf/cpp_sscanf.h \
           $$ROOT/libraries/cpp_sscanf/parser_trait.h \
           $$ROOT/libraries/cpp_sscanf/regex_trait.h \
           $$ROOT/libraries/cpp_sscanf/tokenize.h \
           $$ROOT/libraries/multiple_dispatch/cpp11compatibility.h \
           $$ROOT/libraries/multiple_dispatch/multiple_dispatch_lib.h \
           $$ROOT/libraries/pave/pave.h \
           $$ROOT/libraries/string_matcher/string_matcher.h \
           $$ROOT/obstacle_immobile/brique/brique.h \
           $$ROOT/obstacle_immobile/paroi/paroi.h \
           $$ROOT/obstacle_immobile/sol/sol.h \
           $$ROOT/systeme/systeme/billard.h \
           $$ROOT/viewer/text_viewer/text_viewer.h \

SOURCES += $$ROOT/boule/boule.cpp \
           $$ROOT/collision/collision.cpp \
           $$ROOT/collision/detection_collision.cpp \
           $$ROOT/collision/gestion_collision.cpp \
           $$ROOT/integrateur/integrateur.cpp \
           $$ROOT/systeme/billard_general.cpp \
           $$ROOT/vecteur/vecteur.cpp \
           $$ROOT/viewer/object_viewer.cpp \
           $$ROOT/libraries/multiple_dispatch/exemple_simple.cpp \
           $$ROOT/obstacle_immobile/brique/brique.cpp \
           $$ROOT/obstacle_immobile/paroi/paroi.cpp \
           $$ROOT/obstacle_immobile/sol/sol.cpp \
            $$ROOT/obstacle_immobile/trou/trou.cpp \
           $$ROOT/viewer/text_viewer/text_viewer.cpp \
