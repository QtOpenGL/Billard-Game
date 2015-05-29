//  Fichier : test_collision.cpp
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
#include <vector>
#include "bandit.h"
#include "tests_helper.h"


#include "boule.h"
#include "detection_collision.h"
#include "paroi.h"
#include "brique.h"
#include "sol.h"
#include "trou.h"
#include "integrateur.h"

using namespace std;
using namespace BouleFactory;

// Quelques constantes et fonctions utilitaires

const Vecteur e0{0,0,0};
const Vecteur e1{1,0,0};
const Vecteur e2{0,1,0};
const Vecteur e3{0,0,1};

Boule boule(Vecteur position = Vecteur{0,0,0}, Vecteur vitesse = Vecteur{0,0,0});
Paroi paroi();
Brique brique();
Sol sol();
Trou trou();
void AssertEpsilon(Vecteur const& v1, Vecteur const& v2);
void AssertEpsilon(double d1, double d2);
void AssertTrue(bool to_test);
void AssertFalse(bool to_test);


// Debut des tests

BEGIN_BANDIT
Feature("on veut la distance entre 2 boules ")
{
    When(" elle ne se touchent  pas ")
    {
        Boule b1 = boule();
        Boule b2 = boule(3*e1);
        
        AssertEpsilon(distance_entre(b1, b2), 1.0);
    });
    
    When("elles se touchent")
    {
        Boule b1 = boule();
        Boule b2 = boule(2*e1);
        
        AssertEpsilon(distance_entre(b1, b2), 0.0);
    });
    
    When("elles s'intercectent")
    {
        Boule b1 = boule();
        Boule b2 = boule(e1);
        
        AssertEpsilon(distance_entre(b1, b2), 0.0);
    });
});

Feature("on veut savoir si deux boules se choquent")
{
    When("elles ne se choquent pas")
    {
        Boule b1 = boule();
        Boule b2 = boule(3*e1+3*e2);
        
        AssertFalse(se_choquent(b1, b2));
    });
    When("elles se choquent")
    {
        Boule b1 = boule();
        Boule b2 = boule(2*e1);
        
        AssertTrue(se_choquent(b1, b2));
        
    });
    When("elles s'intersectent")
    {
        Boule b1 = boule();
        Boule b2 = boule(1*e1);
        
        AssertTrue(se_choquent(b1, b2));
    });
});

Feature("collision Boule | Boule : ")
{
    const double dt = 1.0;
    const double temps_depart = 0.0;
    const double N = -1;
    Integrateur_Euler inte;
    
    // Cette structure représente un cas de test et le résultat attendu
    struct Test{
        Boule b1;
        Boule b2;
        bool collision;
        double dt_collision; // N signifie : ne pas tester dt
        string descriptif;
    };
    
    // Cette fonction effectue tous les tests contenus dans un vector<Test>
    auto executer_tests = [&](vector<Test>& tests){
        for(auto& test : tests){
            It(test.descriptif.c_str()){
                auto resultat = dt_jusque_collision(test.b1, test.b2, temps_depart, dt, inte);
                if(test.collision){
                    AssertTrue(resultat.second);
                    if(test.dt_collision != N){
                        AssertEpsilon(resultat.first, test.dt_collision);
                    }
                }else{
                    AssertFalse(resultat.second);
                }
            });
        }
    };
    
    // Ici, nous indiquons les cas qui ne sont pas des vraies collisions
    Describe("[Pas Collision]"){
        vector<Test> tests{
            {boule(-2*e1)     , boule(+2*e1)     , false, 0, "pas contact et vitesses nulles"},
            {boule(-2*e1,  e3), boule(+2*e1,  e3), false, 0, "pas contact, vitesses non nulles"},
            
            {boule(-1*e1)     , boule(+1*e1)     , false, 0, "contact et vitesses nulles"},
            {boule(-1*e1, -e1), boule(+1*e1,  e1), false, 0, "contact mais séparation"},
            {boule(-1*e1)     , boule(+1*e1,  e1), false, 0, "contact mais l'une se sépare"},
            {boule(-1*e1, -e1), boule(+1*e1)     , false, 0, "contact mais l'autre se sépare"},
            {boule(-1*e1,  e1), boule(+1*e1,  e1), false, 0, "contact mais déplacements identiques"},
            
            {boule(-4*e1,  e1), boule(+4*e1, -e1), false, 0, "pas collision avant fin du dt"},
            {boule(-4*e1,  e1), boule(+4*e1,  e1), false, 0, "pas contact et déplacements identiques"},
        };
        executer_tests(tests);
    });
    
    // Ici, nous indiquons quels cas sont considérés comme des collisions
    Describe("[Collision]"){
        vector<Test> tests{
            {boule(-1*e1,  e1), boule(1*e1,  -e1), true,  0 , "contact puis collision frontale"},
            {boule(-1*e1,  e1), boule(  e1      ), true,  0 , "contact puis collision avec boule immobile"},
            
            {boule(-1*e1,  e1), boule(2*e1      ), true,  1 , "collision avec boule immobile"},
            {boule(-2*e1,  e1), boule(2*e1,  -e1), true,  1 , "collision frontale"},
            {boule( 2*e1, -e1), boule(2*e2,  -e2), true,  N , "collision latérale"},
            {boule(   e0,2*e1), boule(3*e1,   e1), true,  1 , "poursuite"}, /*dt doit être < 1 */
            {boule(-2*e1,3*e1), boule(2*e1,-3*e1), true,1./3, "collision avant fin du dt"}
            
        };
        executer_tests(tests);
    });
});

Feature("collision Boule | Paroi : ")
{
    const double dt = 1.0;
    const double temps_depart = 0.0;
    const double N = -1;
    Integrateur_Euler inte;
    Paroi p = paroi();
    
    // Cette structure représente un cas de test et le résultat attendu
    struct Test{
        Boule b1;
        bool collision;
        double dt_collision; // N signifie : ne pas tester dt
        string descriptif;
    };
    
    // Cette fonction effectue tous les tests contenus dans un vector<Test>
    auto executer_tests = [&](vector<Test>& tests){
        for(auto& test : tests){
            It(test.descriptif.c_str()){
                auto resultat = dt_jusque_collision(test.b1, p, temps_depart, dt, inte);
                if(test.collision){
                    AssertTrue(resultat.second);
                    if(test.dt_collision != N){
                        AssertEpsilon(resultat.first, test.dt_collision);
                    }
                }else{
                    AssertFalse(resultat.second);
                }
            });
        }
    };
    
    // Ici, nous indiquons les cas qui ne sont pas des vraies collisions
    Describe("[Pas Collision]"){
        vector<Test> tests{
            {boule(-  e1)     , false, 0, "pas contact et vitesse nulle"},
            {boule(-  e1, -e1), false, 0, "pas contact et séparation"},
            
            {boule(   e3)     , false, 0, "contact et vitesse nulle"},
            {boule(   e3,  e3), false, 0, "contact mais séparation"},
            {boule(   e3, -e1), false, 0, "contact mais déplacement parallèle"},
            {boule(-.5*e3    ), false, 0, "intersection et vitesse nulle"},
            
            {boule(+4*e3, -e3), false, 0, "pas collision avant fin du dt"},
            {boule(-.5*e3-e1,  e1), false,  1 , "collision latérale"},
            
        };
        executer_tests(tests);
    });
    
    // Ici, nous indiquons quels cas sont considérés comme des collisions
    Describe("[Collision]"){
        vector<Test> tests{
            {boule(    e3   , -e3), true,  0 , "contact puis collision frontale"},
            {boule(  2*e3   , -e3), true,  1 , "collision frontale"},
            {boule(1.1*e3   , -e3), true,  N, "collision frontale avant fin du dt"},
            {boule(2*e3-e1, -2*e3+2*e1),true ,1, "collision de biais"},
            
        };
        executer_tests(tests);
    });
});
Feature("Collision Boule/Trou"){
    const double dt = 1.0;
    const double temps_depart = 0.0;
    const double N = -1;
    Integrateur_Euler inte;
    Trou t = trou();
    
    // Cette structure représente un cas de test et le résultat attendu
    struct Test{
        Boule b1;
        bool collision;
        double dt_collision; // N signifie : ne pas tester dt
        string descriptif;
    };
    
    // Cette fonction effectue tous les tests contenus dans un vector<Test>
    auto executer_tests = [&](vector<Test>& tests){
        for(auto& test : tests){
            It(test.descriptif.c_str()){
                auto resultat = dt_jusque_collision(test.b1, t, temps_depart, dt, inte);
                if(test.collision){
                    AssertTrue(resultat.second);
                    if(test.dt_collision != N){
                        AssertEpsilon(resultat.first, test.dt_collision);
                    }
                }else{
                    AssertFalse(resultat.second);
                }
            });
        }
    };
    
    // Ici, nous indiquons les cas qui ne sont pas des vraies collisions
    Describe("[Pas Collision]"){
        vector<Test> tests{
            {boule(-  e1)     , false, 0, "pas contact et vitesse nulle"},
            {boule(-  e1, -e1), false, 0, "pas contact et séparation"},
            
            {boule(   e3)     , false, 0, "contact et vitesse nulle"},
            {boule(   e3,  e3), false, 0, "contact mais séparation"},
            {boule(   e3, -e1), false, 0, "contact mais déplacement parallèle"},
            {boule(-.5*e3    ), false, 0, "intersection et vitesse nulle"},
            
            {boule(+4*e3, -e3), false, 0, "pas collision avant fin du dt"},
            
        };
        executer_tests(tests);
    });
    
    // Ici, nous indiquons quels cas sont considérés comme des collisions
    Describe("[Collision]"){
        vector<Test> tests{
            {boule(    e3   , -e3), true,  0 , "contact puis collision frontale"},
            {boule(  2*e3   , -e3), true,  1 , "collision frontale"},
            {boule(1.1*e3   , -e3), true,  N, "collision frontale avant fin du dt"},
            {boule(2*e3-e1, -2*e3+2*e1),true ,1, "collision de biais"},
            {boule(-.5*e3-e1,  e1), true,  N , "collision latérale"},
            
        };
        executer_tests(tests);
    });
});
END_BANDIT
BANDIT_RUN_TESTS();

Boule boule(Vecteur position, Vecteur vitesse)
{
    Boule boule_source{1, 1, 1, 1};
    boule_source.set_position(position);
    boule_source.set_vitesse(vitesse);
    boule_source.set_resultante_des_forces(e0);
    return boule_source;
}

Paroi paroi()
{
    return Paroi(e0, e1, e2, 1,1);
}

Brique brique()
{
    return Brique(e0, e1, e2, e3);
}

Sol sol()
{
    return Sol(e0, e1, e2, 1, 1);
}
Trou trou()
{
    return Trou(e0, 1, e3);
}

//=====================================================
// Implementation des fonctions utilitaires
//=====================================================

void AssertEpsilon(Vecteur const& v1, Vecteur const& v2)
{
    AssertThat( v1, EpsilonEquals(v2, 10) );
};
void AssertEpsilon(double d1, double d2)
{
    AssertThat( d1, EpsilonEquals(d2, 10) );
};
void AssertTrue(bool to_test)
{
    AssertThat(to_test, Is().True());
}
void AssertFalse(bool to_test)
{
    AssertThat(to_test, Is().False());
}
