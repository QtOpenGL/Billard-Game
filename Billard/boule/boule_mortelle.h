//  Fichier : boule.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Cette boule supprime les boules avec lesquelles elle entre en collision
//

#ifndef Billard_boule_mortelle_h
#define Billard_boule_mortelle_h

#include "boule.h"

class BouleMortelle : public Boule{
public:
    using Boule::Boule;
    OBJET_VISITABLE
};

#endif
