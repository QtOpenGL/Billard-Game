//  Fichier : exercice P11
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :

#include <iostream>
#include <stdexcept>
#include <memory>
#include "objet_mobile.h"
#include "objet.h"
#include "paroi.h"
#include "sol.h"
#include "boule.h"
#include "billard_general.h"
#include "integrateur.h"
#include "text_viewer.h"
#include <memory>
#include "coefficients_physiques.h"
#include <vector>

using namespace std;

BillardAlgo2 initialisation(){
    cout << "------------------------------------------------------------" << endl;
    cout << "déclaration d'un sol/table de jeux " << endl;
    
    const Vecteur origine {0,0,0};
    const Vecteur largeur { 3.569, 0,0};
    const Vecteur longueur{0, 1.778, 0};
    
    Sol* s = new Sol (origine, largeur, longueur, P11::fr_sol, P11::res_sol, P11::roul_sol, P11::glis_sol);
    
    cout << "affichage du sol " << endl;
    cout << s << endl;

    cout << "------------------------------------------------------------" << endl;
    cout << "déclaration des parois " << endl;
    
    //Paroi(Vecteur origine, Vecteur largeur, Vecteur longueur, double coef_r, double frot)
    const Vecteur lar_p{0,0,1};
    
    Paroi* p1 = new Paroi (longueur, lar_p, largeur,  P11::res_bandes, P11::fr_bandes);
    Paroi* p2 = new Paroi (origine,  lar_p, largeur,  P11::res_bandes, P11::fr_bandes);
    Paroi* p3 = new Paroi (origine,  lar_p, longueur, P11::res_bandes, P11::fr_bandes);
    Paroi* p4 = new Paroi (largeur,  lar_p, longueur, P11::res_bandes, P11::fr_bandes);
    
    cout << "afficher une paroi : " << endl;
    cout << p1 << endl;
    
    cout << "------------------------------------------------------------" << endl;
    cout << "déclaration du billard  : " << endl;
    
    BillardAlgo2 billard{unique_ptr<Sol>{s},unique_ptr<Integrateur> {new Integrateur_Euler} };
    
    /*
    billard.ajouter(unique_ptr<Paroi>{p1});
    billard.ajouter(unique_ptr<Paroi>{p2});
    billard.ajouter(unique_ptr<Paroi>{p3});
    billard.ajouter(unique_ptr<Paroi>{p4});
    */
    
    return billard;
}
Boule* obtenir_boule_de_base(){

    Boule* b = new Boule (P11::rayon_boule, P11::masse_boule, P11::fr_boule, P11::res_boule);

    return b;
}

Boule obtenir_boule_simple()
{
    Boule b (P11::rayon_boule, P11::masse_boule, P11::fr_boule, P11::res_boule);
    return b;
}

void avancer_le_temps(Billard& billard, double dt, double temps_max){
    
    TextViewer viewer;
    for(double temps_passe = 0; temps_passe < temps_max; temps_passe += dt){
        cout << "t = " << temps_passe << endl;
        cout << "================================" << endl;
        
        billard.evoluer(dt);
        billard.se_dessiner(viewer);
    }
}


void test1(){
    
    BillardAlgo2 billard = initialisation();
    
    Boule* b = obtenir_boule_de_base();
    b->set_position(Vecteur {0.3, 0, 1.065});
    
    billard.ajouter( unique_ptr<Boule>{b} );
    
    
    cout << "------------------------------------------------------------" << endl;
    cout << "1er test : " << endl;
    cout << "chute libre depuis une hauteur de 1.065 et avec une vitesse nulle" << endl;
    
    avancer_le_temps(billard, 0.01, 0.1);
    
    Boule b1_voule = obtenir_boule_simple();
    
    b1_voule.set_etat(Vecteur{0.3, 0, 1.01104, 0, 0, 0});
    b1_voule.set_vitesse(Vecteur{0, 0, -0.981});
    b1_voule.set_vit_angulaire(Vecteur{0, 0, 0 });
    b1_voule.set_resultante_des_forces(Vecteur{0, 0, -1.24587 });
    
    
    
    if ((b->epsilon(b1_voule, 10) ))
    {
        cout << "le test 1 a réussi " << endl;
    }
    
    
}

void test2()
{
    BillardAlgo2 billard = initialisation();
    
    Boule* b = obtenir_boule_de_base();
    Boule* b2 = obtenir_boule_de_base();
    
    b2->set_position(Vecteur {0.08, 0, 1.16});
    b2->set_vit_angulaire(Vecteur{0,5,0});
    b->set_position(Vecteur{0,0,1});
    b->set_vitesse(Vecteur{1, 0, 2});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "2eme test : " << endl;
    cout << "Dans les deux cas nous avons 2 boules en chute libre qui s'entre-choquent "
    << "une des deux boules a une vitesse de rotation qui entraîne la première boule." << endl;

    
    billard.ajouter(unique_ptr<Boule>{b});
    billard.ajouter(unique_ptr<Boule>{b2});
    
    avancer_le_temps(billard, 0.01, 0.09);
    
}

void test3()
{
    
    BillardAlgo2 billard = initialisation();
    
    Boule* b = obtenir_boule_de_base();
    b->set_position(Vecteur{-1, 0, 0.02625});
    b->set_vitesse(Vecteur{2,0,0});
    b->set_vit_angulaire(Vecteur{0,76.1905,0});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "3eme test : " << endl;
    cout << "Une balle roule : " << endl;
    
    billard.ajouter(unique_ptr<Boule>{b});
   
    avancer_le_temps(billard, 0.01, 0.26);
}

void test4()
{
    
    BillardAlgo2 billard = initialisation();
    
    Boule* b = obtenir_boule_de_base();
    Boule* b2 = obtenir_boule_de_base();
    
    b->set_position(Vecteur {-1, 0, 0.02625});
    b->set_vitesse(Vecteur{2, 0, 0});
    b->set_vit_angulaire(Vecteur{0,76.1905,0});
    b2->set_position(Vecteur{0.5, 0, 0.02625});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "4eme test : " << endl;
    cout << "Une balle roulant, choc une autre balle :" << endl;
    
    billard.ajouter(unique_ptr<Boule>{b});
    billard.ajouter(unique_ptr<Boule>{b2});
    
    avancer_le_temps(billard, 0.01, 0.94);
}

void test5()
{
    
    BillardAlgo2 billard = initialisation();
    
    Boule* b = obtenir_boule_de_base();
    Boule* b2 = obtenir_boule_de_base();
    Boule* b3 = obtenir_boule_de_base();
    
    b->set_position(Vecteur{-1,0,0.02625});
    b->set_vitesse(Vecteur{2,0,0});
    b->set_vit_angulaire(Vecteur{0, 76.1905, 0});
    
    b2->set_position(Vecteur{0.5525, 0, 0.02625});
    
    b3->set_position(Vecteur{0.5,0,0.02625});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "5eme test : " << endl;
    cout << "Balles roulant choc 2 balles : " << endl;


    billard.ajouter(unique_ptr<Boule>{b});
    billard.ajouter(unique_ptr<Boule>{b2});
    billard.ajouter(unique_ptr<Boule>{b3});

    avancer_le_temps(billard, 0.01, 0.28);
    
}

void test6()
{
    BillardAlgo2 billard = initialisation();
    Boule* b = obtenir_boule_de_base();
    
    b->set_position(Vecteur{-1,0,0.02625});
    b->set_vitesse(Vecteur{0.85, 0,0});
    b->set_vit_angulaire(Vecteur{0, -100,0});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "6eme test : " << endl;
    cout << "Effet Retro : " << endl;
    
    billard.ajouter(unique_ptr<Boule>{b});
    
    avancer_le_temps(billard, 0.01, 0.17);

}

void test7()
{
    BillardAlgo2 billard = initialisation();
    Boule* b = obtenir_boule_de_base();
    Boule* b2 = obtenir_boule_de_base();
    
    b->set_position(Vecteur{-1,0,0.02625});
    b->set_vitesse(Vecteur{0.85, 0,0});
    b->set_vit_angulaire(Vecteur{0, -100,0});
    
    b2->set_position(Vecteur{0.5, 0, 0.02625});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "7eme test : " << endl;
    cout << "Effet Retro avec choc : " << endl;
    
    billard.ajouter(unique_ptr<Boule>{b});
    billard.ajouter(unique_ptr<Boule>{b2});
    
    avancer_le_temps(billard, 0.01, 3.22);

}

void test8()
{
    BillardAlgo2 billard = initialisation();

    vector<Boule*> boules;
    
    for (int i (0); i < 11; ++i)
    {
        boules.push_back(obtenir_boule_de_base());
    }
    
    
    
    boules[0]->set_position(Vecteur{-1,0,0.02625});
    boules[0]->set_vitesse(Vecteur{2, 0,0});
    boules[0]->set_vit_angulaire(Vecteur{0, 76.1905,0});
    
    boules[1]->set_position (Vecteur {0.641595, -0.08175, 0.02625});
    boules[2]->set_position (Vecteur {0.641595, -0.02725, 0.02625});
    boules[3]->set_position (Vecteur {0.641595,  0.02725, 0.02625});
    boules[4]->set_position (Vecteur {0.641595,  0.08175, 0.02625});
    boules[5]->set_position (Vecteur {0.594397, -0.0545 , 0.02625});
    boules[6]->set_position (Vecteur {0.594397,  0      , 0.02625});
    boules[7]->set_position (Vecteur {0.594397,  0.0545 , 0.02625});
    boules[8]->set_position (Vecteur {0.547198,  0.02725, 0.02625});
    boules[9]->set_position(Vecteur {0.547198, -0.02725, 0.02625});
    boules[10]->set_position(Vecteur {0.5     ,  0      , 0.02625});
    
    cout << "------------------------------------------------------------" << endl;
    cout << "8eme test : " << endl;
    cout << "Balles roulant choc 10 balles: " << endl;
    
    for(int i (0); i < 11 ; ++i)
    {
        billard.ajouter(unique_ptr<Boule>{boules[i]});
    }
    
    avancer_le_temps(billard, 0.01, 0.76);

}


int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    
    return 0;
}
