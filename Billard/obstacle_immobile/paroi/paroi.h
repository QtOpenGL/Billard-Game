//  Fichier : paroi.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//


#ifndef __Billard__paroi__
#define __Billard__paroi__

#include <stdio.h>
#include "vecteur.h"
#include "obstacle_immobile.h"

class Paroi : public ObstacleImmobile{
public:
    OBJET_VISITABLE
    
    
    Paroi(Vecteur origine, Vecteur largeur, Vecteur longueur, double coef_r = 1, double frot = 1);
    Paroi(Paroi const& p);
    //Paroi& operator=(Paroi const& p);
    //Methodes specifiques à la paroi
    Vecteur get_largeur   () const { return largeur*e1; }
    Vecteur get_longueur  () const { return longueur*e2; }
    
    Vecteur normal_unitaire_a_la_paroi() const;
    PointEtPlan point_le_plus_proche_de(Vecteur const& position) const;
    
protected:
    // Attention à respecter cet ordre de déclaration pour les vecteurs
    
    Vecteur e1;
    Vecteur e2;
    Vecteur e3;

    double largeur;
    double longueur;
};

std::ostream& operator << (std::ostream& flux, Paroi const& paroi);


#endif /* defined(__Billard__paroi__) */
