//  Fichier : exercice P5
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :

#include <iostream>
#include <stdexcept>
#include <memory>
#include "vecteur.h"
#include "boule.h"




using namespace std; 

Boule initialisation_boule()
{
    const double rayon = 0.02625;
    const double masse = 0.127;
    const double coef_frot_propre = 0.2;
    const double coef_restitution = 0.99;
    
    Boule b (rayon, masse, coef_frot_propre, coef_restitution);

    return b;
}

Boule boule1()
{

    Boule b1 = initialisation_boule();
    b1.set_position(Vecteur{-1, 0, 0.02625});
    b1.set_vitesse(Vecteur{2, 0, 0});
    b1.set_vit_angulaire(Vecteur{0, 76.1905, 0});
    
    return b1;
}

Boule boule2()
{
    Boule b = initialisation_boule();
    b.set_position(Vecteur{0.5525, 0, 0.02625});
    
    return b;
}

Boule boule3()
{
    Boule b = initialisation_boule();
    b.set_position(Vecteur{0.5, 0, 0.02625});
    
    return b;
}

int main() {
    
    Boule b1 = boule1();
    Boule b2 = boule2();
    Boule b3 = boule3();
    
    cout << b1 << endl;
    cout << b2 << endl;
    cout << b3 << endl;
    
    return 0;
}
