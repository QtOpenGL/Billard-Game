//  Fichier : integrateur_tests.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour la class intégrateur
//

#include <iostream>
#include <stdexcept>
#include <memory>
#include <utility>
#include "bandit.h"
#include "tests_helper.h"
#include "integrateur.h"
#include "boule.h"
using namespace std;

//
// Here we define some helper functions and structs
//

void AssertEpsilon(double d1, double d2);
void AssertEpsilon(Vecteur const& v1, Vecteur const& v2);
void AssertEpsilon(Boule   const& d1, Boule   const& d2);

unique_ptr<Integrateur> obtenir_integrateur();

//
// Here is where the tests take place
//

BEGIN_BANDIT
Describe("un integrateur"){
    
    It("peut être créé"){
        unique_ptr<Integrateur> p_integrateur = obtenir_integrateur();
    });
    
    Feature("met correctement à jour une Boule"){
        double pas_de_temps = 1/*s*/;
        auto p_integrateur = obtenir_integrateur();

        When("la boule est intialisée"){
            //
            // création de la boule initiale
            //
            Vecteur etat {0,0,0 , 0,0,0};
            Vecteur derive_etat {1,0,0 , 0,0,0};
            Boule   boule_initiale = BouleFactory::boule(1/*cm*/, etat, derive_etat);
            boule_initiale.set_resultante_des_forces({0,0,-9.81});
            
            Vecteur etat_res {1,0,-9.81 , 0,0,0};
            Vecteur derive_etat_res {1,0,-9.81 , 0,0,0}; //pas de frottement, mais gravité
            Boule   resultat = BouleFactory::boule(1/*cm*/, etat_res, derive_etat_res);
            resultat.set_resultante_des_forces({0,0,-9.81});
            
            p_integrateur->integre(boule_initiale, 0/*s*/, pas_de_temps/*s*/);
            
            AssertEpsilon(boule_initiale, resultat);
        });
    });
});
END_BANDIT
BANDIT_RUN_TESTS();


// Les fonctions utilitaires

void AssertEpsilon(Vecteur const& v1, Vecteur const& v2){
    AssertThat( v1, EpsilonEquals(v2, 10) );
};
void AssertEpsilon(double d1, double d2){
    AssertThat( d1, EpsilonEquals(d2, 10) );
};
void AssertEpsilon(Boule const& d1, Boule const& d2){
    AssertThat( d1, EpsilonEquals(d2, 10) );
};

unique_ptr<Integrateur>
obtenir_integrateur()
{
    return unique_ptr<Integrateur>{ new Integrateur_Euler{} };
}
