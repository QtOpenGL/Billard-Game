//  Fichier : exercice P12
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :

#include <iostream>
#include <stdexcept>
#include <memory>
#include "objet.h"
#include "paroi.h"
#include "sol.h"
#include "boule.h"
#include "billard_general.h"
#include "integrateur.h"
#include "text_viewer.h"
#include <memory>
#include "coefficients_physiques.h"
#include "billard_builder.h"

#include "trou.h"
#include "controleur.h"

using namespace std;

//
// Quelques constantes
//
// (Note : nous tolérons les variables globales constantes dans un fichier d'exercice
//  mais il n'y en a évidemment pas dans le code du projet)

const Vecteur e1 {1,0,0};
const Vecteur e2 {0,1,0};
const Vecteur e3 {0,0,1};
const Vecteur pos_trou = {1,1,0};
constexpr double rayon_trou = .2;
constexpr double rayon_boule = .1;

BillardAlgo2 initialisation();

int main()
{
    //
    // CONSTRUCTION
    //
    BillardAlgo2 billard = initialisation();
    
    Boule* b = new Boule(rayon_boule, P11::masse_boule, P11::fr_boule, P11::res_boule);
    b->set_position(pos_trou + (rayon_trou + rayon_boule) * (e1+e2) + rayon_boule * e3);
    b->set_vitesse({ -e1 -e2 });
    billard.ajouter(unique_ptr<Boule>{b});
    
    //
    // EXPLICATIONS
    //
    cout << "Evolution d'une boule sur une table comportant un trou" << endl;
    cout << "La boule est tombée dans le trou vers t=.18" << endl;
    cout << "On vérifie en effet qu'elle ne s'affiche plus pour t=.19" << endl;
    cout << endl << endl;
    
    //
    // EVOLUTION
    //
    double t_a_faire = 0.19;
    double dt = 0.01;
    
    TextViewer viewer;
    billard.se_dessiner(viewer);
    controler(billard, viewer, t_a_faire, dt);
    
    return 0;
}

BillardAlgo2 initialisation(){
    //cout << "------------------------------------------------------------" << endl;
    //cout << "Nouvelle table de jeux " << endl << endl;
    
    const Vecteur origine {0,0,0};
    const Vecteur largeur {3,0,0};
    const Vecteur longueur{0,3,0};
    
    //
    // SOL
    //
    Sol* s = new Sol (origine, largeur, longueur, P11::fr_sol, P11::res_sol, P11::roul_sol, P11::glis_sol);
    
    //cout << "Affichage du sol " << endl;
    //cout << *s << endl << endl;
    
    const Vecteur centre_t1   {1,1,0};
    const Vecteur normal_trou {0,0,1};
    const double  rayon_trou = 0.2;
    
    //
    // TROU
    //
    Trou* t1 = new Trou(centre_t1, rayon_trou , normal_trou);
    
    //cout << "Affichage du trou " << endl;
    //cout << *t1 << endl << endl;
    
    
    BillardAlgo2 billard{unique_ptr<Sol>{s},unique_ptr<Integrateur> {new Integrateur_Euler} };
    billard.ajouter(unique_ptr<ObstacleImmobile> {t1} );
    
    return billard;
}

