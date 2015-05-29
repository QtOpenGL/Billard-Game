//  Fichier : paroi.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//


#include <iostream>
#include "trou.h"
using namespace std;

PointEtPlan Trou::
point_le_plus_proche_de(Vecteur const& point) const
{
    Vecteur O = get_position();
    Vecteur point_dans_plan = point + produit_scalaire(O-point, normal) * normal;
    Vecteur OP = point_dans_plan - O;
    Vecteur normal_collision = unitaire(normal+OP);
    
    if(norme_carree(OP) < rayon*rayon){
        return {point_dans_plan,normal_collision};
    }
    
    Vecteur proche = O + rayon * unitaire(OP);
    return {proche, normal_collision};
}









