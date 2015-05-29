//  Fichier : brique.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//

#include <iostream>
#include <vector>
#include <algorithm>
#include "brique.h"
#include "paroi.h"
#include "obstacle_immobile.h"
#include "almost_equal.h"

using namespace std;

Brique::
Brique(Vecteur position, Vecteur largeur, Vecteur longueur, Vecteur hauteur, double frot_rest, double frot_propre)
: ObstacleImmobile{position, produit_mixte(largeur, longueur, hauteur), frot_rest, frot_propre}
, largeur(largeur)
, longueur(longueur)
, hauteur(hauteur)
{
    //la vérification des dimensions est faite par le pavé
    parois = {
        Paroi(position, longueur, largeur, frot_rest, frot_propre), // vers le bas
        Paroi(position+hauteur, largeur, longueur, frot_rest, frot_propre), //vers le haut
        Paroi(position, hauteur, longueur, frot_rest, frot_propre), //vers la gauche
        Paroi(position+largeur, longueur, hauteur, frot_rest, frot_propre), //vers la droite
        Paroi(position, largeur, hauteur, frot_rest, frot_propre), //vers l'avant
        Paroi(position+longueur, hauteur, largeur, frot_rest, frot_propre), //vers l'arrière
    };
}

PointEtPlan Brique::
point_le_plus_proche_de(Vecteur const& point) const
{
    auto lt = [&](PointEtPlan const& a, PointEtPlan const& b){
        return norme_carree(a.point - point) <= norme_carree(b.point - point);
    };
    
    PointEtPlan zone_plus_proche = parois.front().point_le_plus_proche_de(point);
    
    for(auto& paroi : parois){
        PointEtPlan zone_proche = paroi.point_le_plus_proche_de(point);
        if( lt(zone_proche, zone_plus_proche) ){
            zone_plus_proche = zone_proche;
        }
    }
    
    return zone_plus_proche;
}

std::ostream& operator << (std::ostream& flux, Brique const& b)
{
    flux << "une brique  : " << "#" << endl;
    flux << "sa position : " << b.get_position() << "#" << endl;
    flux << "de hauteur  : " << b.get_hauteur() << "#" << endl;
    flux << "de largeur  : " << b.get_largeur() << "#" << endl;
    flux << "de longueur : " << b.get_longueur() << "#" << endl;
    flux << "avec un coef de frot prpore : "       << b.get_coef_frot_propre()    << "#" << endl;
    flux << "avec un coef de frot de restitution :"<< b.get_coef_frot_restitution()<<"#" << endl;

    return flux;
}





