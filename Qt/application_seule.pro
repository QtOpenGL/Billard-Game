TEMPLATE = subdirs

#
# Les target nécessaires pour compiler l'application
#
SUBDIRS +=          \
    Billard         \
    Billard_OpenGL  \
    Billard_Texte   \

Billard_OpenGL.depends = Billard
Billard_Texte.depends  = Billard

