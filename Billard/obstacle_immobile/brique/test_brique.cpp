//  Fichier : test_brique.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour les fonctions de collision
//

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "bandit.h"
#include "tests_helper.h"
#include "vecteur.h"
#include "obstacle_immobile.h"
#include "brique.h"
#include "almost_equal.h"

using namespace std;


//
// Here we define some helper functions and structs
//

void AssertEpsilon(Vecteur const& v1, Vecteur const& v2){
    AssertThat( v1, EpsilonEquals(v2, 10) );
};
void AssertEpsilon(double d1, double d2){
    AssertThat( d1, EpsilonEquals(d2, 10) );
};



BEGIN_BANDIT

Describe("une brique"){

    const Vecteur e1{1,0,0};
    const Vecteur e2{0,1,0};
    const Vecteur e3{0,0,1};
    const Vecteur e0{0,0,0};
    
    Feature("On peut créer une brique avec des dimensions non nulles"){
        It("la création ne déclenche pas d'erreur"){
            Brique b{e0, e1, e2, e3, 1, 1};
        });
    });
    Feature("Les dimensions ne sont jamais nulles"){
        When ("la largeur est nulle "){
            AssertThrows(std::exception, Brique b(e0, e0, e2, e3) );
        });
        When ("la longueur est nulle "){
            AssertThrows(std::exception, Brique b(e0, e1, e0, e3) );
        });
        When ("la hauteur est nulle "){
            AssertThrows(std::exception, Brique b(e0, e1, e2, e0) );
        });
    });
    Feature("Une brique ne peut être plate"){
        When ("largeur // longueur"){
            AssertThrows(std::exception, Brique b(e0, e1, e1, e3) );
        });
        When ("largeur // hauteur"){
            AssertThrows(std::exception, Brique b(e0, e1, e2, e1) );
        });
        When ("longueur // hauteur"){
            AssertThrows(std::exception, Brique b(e0, e1, e2, e2) );
        });
    });
    
    const double cfr = .3;
    const double cfp = .7;
    const Brique b{e0, e1, e2, e3, cfr, cfp};
    
    Feature("Ses attributs sont accessibles via les méthodes get"){
        When("en vrac"){
            AssertEpsilon(b.get_position(), e0);
            AssertEpsilon(b.get_largeur() , e1);
            AssertEpsilon(b.get_longueur(), e2);
            AssertEpsilon(b.get_hauteur() , e3);
            AssertEpsilon(b.get_coef_frot_restitution(), cfr);
            AssertEpsilon(b.get_coef_frot_propre(), cfp);
        });
    });
    
    Feature("peut calculer le point P le plus proche d'un point A donné lorsque : "){
        struct Test{
            string description;
            Vecteur input;
            Vecteur expected;
        };
        vector<Test> test_cases{
            // {"le point est dans la brique", {e0 +.5*(e1+e2+e3)}, {e0 +.5*(e1+e2+e3)}},
            // Nous avons décidé de forcer les points proches à être sur la surface
            
            {"le point est sur la surface", {e0}, {e0}},
            {"trop à gauche, trop haut"   , {e0-e1+2*e2}, {e0+e2}},
            {"point au milieu, trop bas"  , {e0+ .5*(e1+e2)-e3}, {e0+ .5*(e1+e2)}}
        };
        for(auto const& test : test_cases){
            When( test.description.c_str() ){
                Vecteur resultat = b.point_le_plus_proche_de(test.input).point;
                AssertEpsilon(resultat, test.expected);
            });
        }
    });
    
    When("on veut afficher une brique"){
#warning "a faire !!!!"
    });
});


END_BANDIT
BANDIT_RUN_TESTS();
