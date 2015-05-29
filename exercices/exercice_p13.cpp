//  Fichier : exercice P13
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
#include "coefficients_physiques.h"

Boule initialiser_boule()
{
    
    Boule b (P11::rayon_boule, P11::masse_boule, P11::fr_boule, P11::res_boule);
    
    b.set_position(Vecteur{0,0,1});
    b.set_vitesse (Vecteur{1,0,2});
    b.set_resultante_des_forces(Vecteur{0, 0, -1.24587});
    
    return b ;
}

void fonctionIntegree(double t, Boule& b1, Integrateur& inte, double temps, double dt)
{
    do
    {
        std::cout << "=======================================================" << std::endl;
        std::cout << "t == " << t << std::endl;
        std::cout << b1 << std::endl;
        
        inte.integre(b1, temps, dt );
        
        t = t + 0.01;
        
    }while (t <= 0.19);
    
}

void initialiser_test(Integrateur& inte)
{
    Boule b1 = initialiser_boule();
    double temps = 1;
    double dt = 0.01;
    double t = 0.0;
    
    fonctionIntegree(t, b1, inte, temps, dt);

}

void testInte_Euler_crommer()
{
    Integrateur_Euler inte;

    std::cout << "inte Euler " << std::endl;
    
    initialiser_test(inte);
}

void testInte_Newmark()
{
    
    Integrateur_Newmark inte;
    
    std::cout << "inte Newmark " << std::endl;
    
    initialiser_test(inte);
}

void testInte_Runge_Kutta_ordre_4()
{
    Integrateur_Runge_Kutta4 inte;
    
    std::cout << "inte Runge " << std::endl;
    
    initialiser_test(inte);
}

void testInte_reel ()
{
    Integrateur_Newmark_Simple inte;
    
    std::cout << "inte reel " << std::endl;
    initialiser_test(inte);
}


int main()
{
    testInte_Euler_crommer();
    testInte_Newmark();
    testInte_reel();
    testInte_Runge_Kutta_ordre_4();
    
}
