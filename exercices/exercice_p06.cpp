//  Fichier : exercice P6
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
#include <iostream>
#include <stdexcept>
#include <memory>
#include "boule.h"
#include "integrateur.h"


Boule initialiser_boule()
{
    const double rayon = 0.02625;
    const double masse = 0.127;
    const double coef_frot_propre = 0.2;
    const double coef_restitution = 0.99;
    
    Boule b (rayon, masse, coef_frot_propre, coef_restitution);
    
    b.set_position(Vecteur{0,0,1});
    b.set_vitesse (Vecteur{1,0,2});
    b.set_resultante_des_forces(Vecteur{0, 0, -1.24587});
    
    return b ;
}

int main() {
   
    Boule b1 = initialiser_boule();
    Integrateur_Euler inte;
    double temps = 1;
    double dt = 0.01;
    double t = 0.0;
    
    do
    {
        std::cout << "=======================================================" << std::endl;
        std::cout << "t == " << t << std::endl;
        std::cout << b1 << std::endl;
        
        inte.integre(b1, temps, dt );
        
        t = t + 0.01;
        
    }while (t <= 0.02);
    
    return 0;
}
