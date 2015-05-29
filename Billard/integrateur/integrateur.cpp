//  Fichier : integrateur.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//

#include <iostream>
#include <array>
#include <tuple>
#include "integrateur.h"
#include "vecteur.h"
#include "newmark.h"
#include "runge_kutta.h"
using namespace std;


void Integrateur_Euler::
integre (Integrable& objet,double temps_depart, double dt_recherche_max) const
{
    Vecteur omega_p = objet.derivee_etat();
    Vecteur omega   = objet.etat();
    
    omega_p = omega_p + dt_recherche_max * objet.derivee_seconde_etat(temps_depart);
    omega   = omega   + dt_recherche_max * omega_p;
    
    // Note j'ai bien vérifié l'ordre, tout est bon
    
    objet.set_derivee_etat(omega_p);
    objet.set_etat(omega);
}

void Integrateur_Newmark::
integre (Integrable& objet,  double temps, double dt) const
{
    Vecteur omega_pp = objet.derivee_seconde_etat(temps);
    Vecteur omega_p  = objet.derivee_etat();
    Vecteur omega    = objet.etat();
    
    auto f = [&](double t, Vecteur const& omega, Vecteur const& omega_p){
        return omega_pp; // Dans notre projet l'accéleration ne dépend pas des autres variables.
    };
    std::tie(omega, omega_p) = algorithme_newmark(omega, omega_p, f, temps, dt, .1, numeric_limits<int>::infinity());
    
    objet.set_derivee_etat(omega_p);
    objet.set_etat(omega);
}


void Integrateur_Newmark_Simple::
integre(Integrable& objet, double temps, double dt) const
{
    // Cette méthode implémente la mise à jour pour le cas où la fonction f ne dépend pas du temps
    
    Vecteur omega_pp = objet.derivee_seconde_etat(temps);
    Vecteur omega_p  = objet.derivee_etat();
    Vecteur omega    = objet.etat();
    
    omega   = omega   + dt * omega_p + dt*dt/2.0 * omega_pp;
    omega_p = omega_p + dt * omega_pp;
    
    // Note j'ai bien vérifié l'ordre : omega(n) est en fonction de omega_p(n-1)
    
    objet.set_derivee_etat(omega_p);
    objet.set_etat(omega);
}


void Integrateur_Runge_Kutta4::
integre(Integrable& objet, double temps, double dt) const
{
    Vecteur omega_pp = objet.derivee_seconde_etat(temps);
    Vecteur omega_p  = objet.derivee_etat();
    Vecteur omega    = objet.etat();
    
    auto f = [&](double t, Vecteur const& omega, Vecteur const& omega_p){
        return omega_pp; // Dans notre projet l'accéleration ne dépend pas des autres variables.
    };
    std::tie(omega, omega_p) = runge_kutta_4(omega,omega_p, f, temps, dt);
    
    objet.set_derivee_etat(omega_p);
    objet.set_etat(omega);
}
