//  Fichier : runge_kutta.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Implémentation de la méthode de Runge-Kutta à l'ordre 4
//

#ifndef __Billard__runge_kutta__
#define __Billard__runge_kutta__

#include <utility>

//
// Méthode de Runge-Kutta à l'ordre 4
//
// f est la fonction d'évolution suivante :
// Omega''(t) = f(t,Omega(t),Omega'(t))
//
// où :
// Omega(t) est le vecteur position,
// Omega'(t)    le vecteur vitesse,
// Omega''(t)   le vecteur acceleration
//
//
// retour :
// La fonction renvoit { Omega(t+dt) , Omega'(t+dt) }
//

#include <utility>
#include <array>

template <class F>
std::pair<Vecteur,Vecteur>
runge_kutta_4(Vecteur omega, Vecteur omega_p, F f, double t, double dt)
{
    double dt_l_2    = dt / 2;
    
    // Déclaration des constantes, initialisées à zéro car Vecteur n'est pas default constructible
    
    Vecteur O{0};
    std::array<Vecteur, 4> k{O,O,O,O};
    std::array<Vecteur, 4> k_p = k;
    
    // Algorithme :
    
    k[0]   = omega_p;
    k_p[0] = f(t, omega, omega_p);
    
    for(int i : {1,2}){
        k[i]   = omega_p + dt_l_2 * k_p[i-1];
        k_p[i] = f(t + dt_l_2, omega + dt_l_2 * k[i-1] , omega_p + dt_l_2 * k_p[i-1]);
    }
    
    k[3]   = omega_p + dt * k_p[2];
    k_p[3] = f(t + dt, omega + dt * k[2] , omega_p + dt * k_p[2]);
    
    omega   = omega   + dt/6.0 * (k[0]   + 2*k[1]   + 2*k[2]   + k[3]  );
    omega_p = omega_p + dt/6.0 * (k_p[0] + 2*k_p[1] + 2*k_p[2] + k_p[3]);
    
    return {omega, omega_p};
}

#endif /* defined(__Billard__runge_kutta__) */
