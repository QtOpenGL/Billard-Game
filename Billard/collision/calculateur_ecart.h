//  Fichier : calculateur_ecart.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//     Ce fichier implémente les fonctionnalitées principales d'un objet
//     Vis-à-vis d'un autre objet.
//     Il s'agit en fait d'une suite de classes de fonctionnalités
//     que nous avons rassemblées dans un meme fichier.
//

#ifndef Billard_calculateur_ecart_h
#define Billard_calculateur_ecart_h

#include <iostream>
#include "objet.h"
#include "boule.h"

//==============================
// Ecarts
//==============================

class CalculateurDeEcart {
public:
    Vecteur entre
    (Boule const& a, Objet const& b);
    //Vecteur operator() (Boule&, Boule&);
    //Vecteur operator() (Boule&, ObstacleImmobile&);
    //Vecteur operator() (ObstacleImmobile&, Boule&);
};

Vecteur CalculateurDeEcart::
entre (Boule const& a, Objet const& b){
    using namespace std;
    Vecteur bp = b.point_le_plus_proche_de(a.get_position()).point;
    Vecteur ap = a.point_le_plus_proche_de(bp).point;
    
    return ap - bp;
}
#endif
