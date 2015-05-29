//  Fichier : paroi.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//


#include <utility>
#include <iostream>
#include <algorithm>
#include "paroi.h"
#include "obstacle_immobile.h"
#include "vecteur.h"
#include "boule.h"
#include "almost_equal.h"

using namespace std;

Paroi::
Paroi(Vecteur origine, Vecteur arg_largeur, Vecteur arg_longueur, double coef_r, double frot)
: ObstacleImmobile(origine, norme(produit_vectoriel(arg_longueur, arg_largeur)), coef_r, frot)
, e1{ unitaire(arg_largeur)    }
, e2{ unitaire(arg_longueur)   }
, e3{ produit_vectoriel(e1, e2)}
{
    //la vérification des dimensions est faite par le pavé
    if( get_volume() == 0){
        throw std::length_error{"Impossible de créer une brique avec des dimensions nulles"};
    }
    largeur = norme(arg_largeur);
    longueur = norme(arg_longueur);
}

Paroi::
Paroi(Paroi const& p)
:Paroi(p.get_position(), p.get_largeur(), p.get_longueur(), p.get_coef_frot_restitution(), p.get_coef_frot_propre())
{}


/*Paroi& Paroi::
operator=(Paroi const& p)
{
    Paroi nouv{p};
    swap(nouv, *this);
    return *this;
}*/
Vecteur Paroi::
normal_unitaire_a_la_paroi() const
{
    return e3;
}

PointEtPlan  Paroi::
point_le_plus_proche_de(Vecteur const& point_exterieur) const
{
    // Cette formule ne marche pas si les axes ne sont pas orthogonaux.

    Vecteur OP = point_exterieur - get_position();
    double x = produit_scalaire(OP, e1);
    x = max(0., x);
    x = min(largeur, x);
    
    double y = produit_scalaire(OP, e2);
    y = max(0., y);
    y = min(longueur, y);
    
    Vecteur proche = get_position() + x * e1 + y * e2;
    return {proche, e3};
}

std::ostream& operator << (std::ostream& flux, Paroi const& paroi)
{
    flux << "une paroi : "                                                        << " # " << std::endl;
    flux << "d'origine : "                   << paroi.get_position()              << " # " << std::endl;
    flux << "de longeure : "                 << paroi.get_longueur ()              << " # " << std::endl;
    flux << "de largueur : "                 << paroi.get_largeur()              << " # " << std::endl;
    flux << "avec un coef de restitution : " << paroi.get_coef_frot_restitution() << " # " << std::endl;
    flux << "avec un coef prpore : "         << paroi.get_coef_frot_propre()      << " # " << std::endl;
    
    return flux;
}









