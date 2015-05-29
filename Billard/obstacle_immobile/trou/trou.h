//  Fichier : paroi.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//


#ifndef __Billard__trou__
#define __Billard__trou__

#include <stdio.h>
#include "vecteur.h"
#include "obstacle_immobile.h"

class Trou : public ObstacleImmobile{
    Vecteur normal;
    double rayon;
public:
    OBJET_VISITABLE
    
    Trou(Vecteur centre, double rayon, Vecteur normal)
    : ObstacleImmobile{centre, 3.14*rayon*rayon, 0, 0}
    , normal{normal}, rayon{rayon}
    {
        if (zero(normal)){
            throw std::logic_error{"Impossible de créer un trou sans vecteur normal"};
        }
        if (rayon == 0){
            throw std::logic_error{"Impossible de créer un trou de rayon nul"};
        }
    }
    
    PointEtPlan point_le_plus_proche_de(Vecteur const& point) const override;
    double get_rayon() const { return rayon; }
    Vecteur get_normal() const { return normal; }
};


#endif /* defined(__Billard__paroi__) */
