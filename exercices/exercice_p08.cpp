//  Fichier : exercice P8
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : instansiation d'un systeme dans le quel on devra mettre 3 boules


#include <iostream>
#include <stdexcept>
#include <memory>
#include "boule.h"
#include "vecteur.h"
#include "integrateur.h"
#include "detection_collision.h"
#include "billard_general.h"
#include "integrateur.h"
#include "text_viewer.h"
#include "object_viewer.h"
#include "gestion_collision.h"
#include <memory>
#include "collision.h"
#include "controleur.h"
#include "sol.h"
#include "paroi.h"
#include <vector>

using namespace std;


Boule initialiser_boule()
{
    const double rayon = 0.02625;
    const double masse = 0.127;
    const double coef_frot_propre = 0.2;
    const double coef_restitution = 0.99;
    
    Boule b (rayon, masse, coef_frot_propre, coef_restitution);
    
    b.set_resultante_des_forces(Vecteur{0, 0, -1.24587});
    
    return b;
}

Boule* boule1 ()
{
    Boule* b = new Boule (initialiser_boule());
    
    b->set_position(Vecteur{-1, 0, 0.02625});
    b->set_vitesse(Vecteur{2, 0, 0});
    b->set_vit_angulaire(Vecteur{0, 76.1905, 0});
    
    return b;

}

Boule* boule2()
{
    
    Boule* b = new Boule (initialiser_boule());
    
    b->set_position(Vecteur{0.5525, 0, 0.02625});
    return b;
}

Boule* boule3()
{
    Boule* b = new Boule (initialiser_boule());
    
    b->set_position(Vecteur{0.5, 0, 0.02625});
    return b;
}

BillardAlgo2 initialisation(){
    
    const Vecteur origine {0,0,0};
    const Vecteur largeur { 3.569, 0,0};
    const Vecteur longueur{0, 1.778, 0};
    
    const double coef_rest          = 0.4 ;
    const double coef_frot          = 0.05;
    const double coef_fr_roulement  = 0.05;
    const double coef_fr_glissement = 0.1 ;
    
    Sol* s = new Sol (origine, largeur, longueur, coef_frot, coef_rest, coef_fr_roulement, coef_fr_glissement);
    
    const double coef_rest_p = 0.9;
    const double coef_frot_p = 0.15;
    const Vecteur lar_p{0,0,1};
    
    Paroi* p1 = new Paroi (longueur, lar_p, largeur,  coef_rest_p, coef_frot_p);
    Paroi* p2 = new Paroi (origine,  lar_p, largeur,  coef_rest_p, coef_frot_p);
    Paroi* p3 = new Paroi (origine,  lar_p, longueur, coef_rest_p, coef_frot_p);
    Paroi* p4 = new Paroi (largeur,  lar_p, longueur, coef_rest_p, coef_frot_p);
    
    vector<Paroi*> parois {p1, p2, p3, p4};
    
    BillardAlgo2 billard{unique_ptr<Sol>{s},unique_ptr<Integrateur> {new Integrateur_Euler} };
    
    
    for (int i (0); i < 4; ++i )
    {
        billard.ajouter(unique_ptr<Paroi>{parois[i]});
    }
    
    return billard;
}

void test()
{
    // création des boules

    vector<Boule*> boules {boule1(), boule2(), boule3()};
    
    // création du systeme qu'on a nommer BillardAglo1
    BillardAlgo2 b = initialisation();
    
    //affection des 3 boules dans billard
    for(int i (0); i<3; ++i)
    {
        b.ajouter(unique_ptr<Boule>{boules[i]});
    }
   
    // affichage du billard avec utilisation du viewer 
    TextViewer viewer;
    b.se_dessiner(viewer);
    
}


int main() {
   
    test();
    
    return 0;
}
