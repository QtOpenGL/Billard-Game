//  Fichier : test_paroi.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour la classe Trou
//

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include "bandit.h"
#include "tests_helper.h"
#include "trou.h"
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

Vecteur e0{0,0,0};
Vecteur e1{1,0,0};
Vecteur e2{0,1,0};
Vecteur e3{0,0,1};

//
// here is where the tests take place
//

BEGIN_BANDIT
Describe("Un trou"){
    It("Ne peut pas etre de rayon nul"){
        AssertThrows(std::exception, Trou(e0, 0, e1));
        
    });
    It("Doit avoir un vecteur normal non nul"){
        AssertThrows(std::exception, Trou(e0, 1, e0));
        
    });
});
Feature("Point le plus proche"){
    // TDD
    
    Vecteur normal{ e2 };
    Trou t{e1, 1, normal};
    struct TestCase{
        Vecteur point_exterieur;
        Vecteur point_proche;
        string description;
    };
    vector<TestCase> tests{
        {e1, e1, "le point est intérieur"},
        {e0, e0, "le point est sur le bord"},
        {e1+e3, e1+e3, "le point est sur le bord (bis)"},
        {e1+e2, e1, "le point est au dessus du cercle"},
        {e1-e2, e1, "le point est au dessous du cercle"},
        {-e1, e0, "le point est à gauche du cercle"},
        {5*e1, 2*e1, "le point est à droite du cercle"},
        {e1+2*e3, e1+e3, "le point est avant le cercle"},
    };
    for(auto& test : tests){
        It(test.description.c_str()){
            auto resultat = t.point_le_plus_proche_de(test.point_exterieur);
            
            AssertEpsilon(resultat.point, test.point_proche);
            
            double direction_selon_normal = produit_scalaire(resultat.normal_au_plan_tangent, normal);
            AssertThat(direction_selon_normal, Is().GreaterThan(0));
            
            //double direction_selon_ecart = produit_scalaire(resultat.normal_au_plan_tangent, test.point_exterieur - e1);
            //AssertThat(direction_selon_ecart, Is().GreaterThan(0).Or().EqualTo(0));
            
        });
    }
});

END_BANDIT
BANDIT_RUN_TESTS();
