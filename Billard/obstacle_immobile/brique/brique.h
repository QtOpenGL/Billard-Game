//  Fichier : brique.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//


#ifndef __Billard__brique__
#define __Billard__brique__

#include <vector>
#include "obstacle_immobile.h"
#include "paroi.h"

class Brique : public ObstacleImmobile {
    Vecteur largeur;
    Vecteur longueur;
    Vecteur hauteur;
    std::vector<Paroi> parois;

public:
    OBJET_VISITABLE
    
    
    Brique(Vecteur position, Vecteur largeur, Vecteur longueur, Vecteur hauteur,
           double frot_rest = 1, double frot_propre = 1);
    
    Vecteur const& get_largeur () const { return largeur;  }
    Vecteur const& get_longueur() const { return longueur; }
    Vecteur const& get_hauteur () const { return hauteur;  }
    
    //methodes specifiques
    PointEtPlan point_le_plus_proche_de(Vecteur const& point) const;
    std::vector<Paroi> const& get_parois() const { return parois; }
};
std::ostream& operator << (std::ostream& flux, Brique const& brique);

#endif /* defined(__Billard__brique__) */
