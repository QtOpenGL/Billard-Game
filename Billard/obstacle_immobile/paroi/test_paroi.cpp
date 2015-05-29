//  Fichier : test_paroi.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour les fonctions de collision
//

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include "bandit.h"
#include "tests_helper.h"
#include "paroi.h"
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


//
// here is where the tests take place
//

BEGIN_BANDIT

Describe("une paroi"){

    const Vecteur e0{0,0,0};
    const Vecteur e1{1,0,0};
    const Vecteur e2{0,1,0};
    const Vecteur e3{0,0,1};
    
    Feature("La création d'une paroi"){
    
        It("réussie si les dimensions sont non nulles"){
            Paroi p (e0, e1, e2);
        });
        
        It("échoue lorsque les dimensions sont nulles"){
            AssertThrows(std::exception, Paroi(e0, e0, e2));
            AssertThrows(std::exception, Paroi(e0, e1, e0));
        });
        
        It("échoue lorsque les dimensions sont colinéaires"){
            AssertThrows(std::exception, Paroi(e0, e1, e1));
        });
    });
    
    const double cfr = .5;
    const double cfp = .7;
    Paroi p{e0, e1, e2, cfr, cfp};
    
    Feature("on peut avoir accès à ses attributs"){
        When("En vrac"){
            AssertEpsilon(p.get_position(), e0);
            AssertEpsilon(p.get_largeur() , e1);
            AssertEpsilon(p.get_longueur(), e2);
            AssertEpsilon(p.get_coef_frot_propre(), cfp);
            AssertEpsilon(p.get_coef_frot_restitution(), cfr);
        });
    });
    
    Feature("On peut obtenir le point le plus proche"){
        struct Test{
            string description;
            Vecteur input;
            Vecteur expected;
        };
        vector<Test> test_cases{
            {"le point est dans la paroi", {e0 +.5*(e1+e2)}, {e0 +.5*(e1+e2)}},
            {"trop à gauche, trop haut"   , {e0-e1+2*e2}, {e0+e2}},
            {"point au milieu, trop bas"  , {e0+ .5*(e1+e2)-e3}, {e0+ .5*(e1+e2)}}
        };
        for(auto const& test : test_cases){
            When( test.description.c_str() ){
                    Vecteur resultat = p.point_le_plus_proche_de(test.input).point;
                    AssertEpsilon(resultat, test.expected);
            });
        }
    });


});


END_BANDIT
BANDIT_RUN_TESTS();
