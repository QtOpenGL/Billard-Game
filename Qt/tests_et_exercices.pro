TEMPLATE = subdirs

#
# Les target nécessaires pour compiler l'application
#
SUBDIRS += \
    Billard \
    Billard_OpenGL \
    Billard_Texte \

Billard_OpenGL.depends = Billard
Billard_Texte.depends  = Billard
#
# Les targets pour les tests
#
SUBDIRS += \
    test_premier_affichage \
    test_glwidget \
    tests/test_vecteur \
    tests/test_boule \
    tests/test_brique \
    tests/test_paroi \
    tests/test_trou \
    tests/test_detection_collision \
    tests/test_gestion_collision \
    tests/test_billard_general \
    tests/test_integrateur_euler \

test_premier_affichage.depends = Billard
test_glwidget.depends = Billard
tests/test_vecteur.depends = Billard
tests/test_boule.depends = Billard
tests/test_brique.depends = Billard
tests/test_paroi.depends = Billard
tests/test_trou.depends = Billard
tests/test_detection_collision.depends = Billard
tests/test_gestion_collision.depends = Billard
tests/test_billard_general.depends = Billard
tests/test_integrateur_euler.depends = Billard

#
# Les targets pour les exercices
#
SUBDIRS += \
    exercices/exercice_p01 \
    exercices/exercice_p04 \
    exercices/exercice_p05 \
    exercices/exercice_p06 \
    exercices/exercice_p07 \
    exercices/exercice_p08 \
    exercices/exercice_p09 \
    exercices/exercice_p11 \
    exercices/exercice_p12 \
    exercices/exercice_p13 \

exercices/exercice_p01.depends = Billard
exercices/exercice_p04.depends = Billard
exercices/exercice_p05.depends = Billard
exercices/exercice_p06.depends = Billard
exercices/exercice_p07.depends = Billard
exercices/exercice_p08.depends = Billard
exercices/exercice_p09.depends = Billard
exercices/exercice_p11.depends = Billard
exercices/exercice_p12.depends = Billard
exercices/exercice_p13.depends = Billard
