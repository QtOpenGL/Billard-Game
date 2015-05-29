//  Fichier : controleur.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__controleur__
#define __Billard__controleur__

#include "billard.h"
#include "viewer.h"

//
// Exemple d'utilisation :
//
// int main(){
//    TextViewer v{}; // Il existe aussi une classe DummyViewer qui n'affiche rien
//    controler(billard, v, 10, .1);
// }
//
inline void
controler(Billard& b, Viewer& v, double temps_a_faire, double dt)
{
    for(double temps_fait = 0; temps_fait <= temps_a_faire; temps_fait += dt)
    {
        b.evoluer(dt);
        b.se_dessiner(v);
        
        std::cout
        << "Tous les objets ont été affichés pour la date : "
        << temps_fait + dt
        << std::endl << std::endl << std::endl;
    
    }
}

#endif /* defined(__Billard__controleur__) */
