//  Fichier : exercice P7
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : nous avons 2 boules en chute libre qui s'entre-choquent. Dans le premier cas elles ne tournent pas sur elles-mêmes. Dans le second cas, une des deux boules a une vitesse de rotation qui entraîne la première boule.
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cmath>
#include "boule.h"
#include "vecteur.h"
#include "integrateur.h"
#include "detection_collision.h"
#include "billard_general.h"
#include "sol.h"
#include "paroi.h"
#include "integrateur.h"
#include "text_viewer.h"
#include "object_viewer.h"
#include "gestion_collision.h"
#include <memory>
#include "collision.h"
#include "controleur.h"
#include "coefficients_physiques.h"
#include <string>

using namespace std;

Boule initialiser_boule()
{
    Boule b (0.02625, 0.127, 0.2, 0.99);
    
    b.mise_a_jour_resultante_des_forces();
    return b;
}

Boule* boule1 ()
{
    
    Boule* b = new Boule (initialiser_boule());
    
    b->set_position(Vecteur{0,0,1});
    b->set_vitesse(Vecteur {1,0,2});
    
    return b;
}

Boule* boule2()
{
    Boule* b = new Boule (initialiser_boule());
    
    b->set_position(Vecteur{0.08, 0, 1.16});
    return b;
}

BillardAlgo2 initialisation(){
    
    const Vecteur origine {0,0,-10};
    const Vecteur largeur { 100, 0,0};
    const Vecteur longueur{0, 100, 0};
    
    const double coef_rest          = 0.4 ;
    const double coef_frot          = 0.05;
    const double coef_fr_roulement  = 0.05;
    const double coef_fr_glissement = 0.1 ;
    
    Sol* s = new Sol (origine, largeur, longueur, coef_frot, coef_rest, coef_fr_roulement, coef_fr_glissement);
    
    BillardAlgo2 billard{unique_ptr<Sol>{s},unique_ptr<Integrateur> {new Integrateur_Euler} };
    
    return billard;
    
}

void test1()
{
    
    cout << "___________test1_______________" << endl;
    
    vector<Boule*> boules{ boule1() , boule2() };
    
    double dt = 0.01;
    double temps_a_faire = 0.07;
    
    BillardAlgo2 b = initialisation();
    TextViewer viewer;
    
    for (int i = 0; i<2 ; ++i)
    {
        b.ajouter( unique_ptr<Boule>{boules[i]});
    }
  
    controler( b, viewer, temps_a_faire, dt);
    
}

void test2()
{
    
    cout << "___________test2_______________" << endl;
    
    
    vector<Boule*> boules{ boule1() , boule2() };
    
    // ajout d'une vitesse angulaire a la deuxieme boule 0 5 0 # vitesse angulaire
    boules[1]->set_vit_angulaire(Vecteur{0,5,0});
    
    double dt = 0.01;
    double temps_a_faire = 0.06;
    
    BillardAlgo2 b = initialisation();
    TextViewer viewer;
    
    for ( int i = 0 ; i < 2 ; ++i)
    {
        b.ajouter( unique_ptr<Boule>{ boules[i] } );
    }
    
    controler( b, viewer, temps_a_faire, dt);
    
}




int main() {
    
    test1();
    test2();
    
    return 0;
}
