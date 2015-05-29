//  Fichier : billard.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Cet interface permet de faire le lien entre l'application billard et
//                les controleurs Qt ou autre.

#ifndef __Billard__billard__
#define __Billard__billard__

#include "viewer.h"

class Billard {
public:
    virtual void evoluer(double dt) = 0;
    virtual void se_dessiner(Viewer& viewer) = 0;
};


#endif /* defined(__Billard__billard__) */
