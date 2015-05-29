//  Fichier : boule_tests.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour la class Boule
//

#include <sstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <regex>
#include <iomanip>

#include "cpp_sscanf.h"
#include "parser_trait.h"
#include "tokenize.h"

#include "bandit.h"
#include "tests_helper.h"

#include "boule.h"
#include "detection_collision.h"
#include "gestion_collision.h"

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

Describe("Une boule")
{
    Feature("La dimension du vecteur d'etat est 6"){
        double dimension = Boule::DIM_ETAT;
        AssertThat(dimension, Equals( 6 ));
    });
    
    Feature("elle peut etre créée"){
        When("le rayon est nul"){
            AssertThrows(std::exception, Boule b(0, 1, 1, 1); );
        });
        When("la masse volumique est nulle"){
            AssertThrows(std::exception, Boule b(1, 0, 1, 1); );
        });
        When("les coefs sont nuls"){
            Boule b(1, 1, 0, 0);
        });
        When("rien n'est nul"){
            Boule b(1, 2, 3, 4);
        });
        When("elle est constante"){
            const Boule b(1,2,3,4);
        });
    });
    
    Feature("on peut avoir acces a ses attributs par des methodes get"){
        
        Feature("apres l'initialisation"){
            
            const Boule b(1,2,3,4);
            
            When("on veut le coef_frot_roulement"){
                double coef_frot_roulement = b.get_coef_frot_roulement();
                AssertEpsilon( coef_frot_roulement , 0 );
            });
            
            When("on veut le coef_frot_glissement"){
                double coef_frot_glissement = b.get_coef_frot_glissement();
                AssertEpsilon( coef_frot_glissement , 0 );
            });
            
            When("on veut le vecteur etat"){
                Vecteur etat = b.etat();
                Vecteur resultat = Vecteur::DeTaille( Boule::DIM_ETAT );
                AssertEpsilon( etat , resultat );
            });
            
            When("on veut le vecteur derivee_etat"){
                Vecteur derivee_etat = b.derivee_etat();
                Vecteur resultat = Vecteur::DeTaille( Boule::DIM_ETAT );
                AssertEpsilon( derivee_etat , resultat );
            });
            
            When("on veut le volume"){
                double volume = b.get_volume();
                double resultat = (4*1*M_PI)/3;
                AssertEpsilon( volume , resultat );
            });
            
            When("on veut la masse"){
                double masse = b.get_masse();
                double resultat = 2;
                AssertEpsilon( resultat , masse );
            });
            
            When("on veut la derivee seconde de l'etat quand elle est nulle"){
                auto resultat  = Vecteur::DeTaille(6);
                
                resultat = b.derivee_seconde_etat(1);
                
                Vecteur attendu {0, 0, 0, 0, 0, 0};
                AssertEpsilon(resultat, attendu);
            });
            When("on veut la derivee seconde de l'etat quand elle est non nulle"){
                Boule boule_source (4, 5, 6, 7);
                boule_source.set_coef_frot_roulement(5);
                boule_source.set_coef_frot_glissement(6);
                boule_source.set_etat( Vecteur{7, 8, 9, 10, 11, 12} );
                boule_source.set_derivee_etat( Vecteur{13, 14, 15, 16, 17, 18} );
                
                Vecteur p_s {0, 0, -9.81};
                boule_source.set_resultante_des_forces(boule_source.get_masse() * p_s );
                
                auto resultat  = Vecteur::DeTaille(6);
                
                resultat = boule_source.derivee_seconde_etat(1);
                
                Vecteur a_s = (produit_vectoriel(boule_source.get_position(), p_s*5)) /(2./5. * 5.00 * 16);
                Vecteur attendu = concatenation(p_s, a_s);
                AssertEpsilon(resultat, attendu);
            });
            
        });
    });
    
    Feature("on peut modifier ses attributs par des methodes set"){
        
        Feature("apres initialisation") {
            
            Boule b(1, 2, 3, 4);
            const double nouveau = 1.3;
            
            When("on donne le coef de roulement "){
                b.set_coef_frot_roulement(nouveau);
                double resultat = b.get_coef_frot_roulement();
                
                AssertEpsilon( resultat , nouveau );
            });
            When("on donne le coef de glissement"){
                b.set_coef_frot_glissement(nouveau);
                double resultat = b.get_coef_frot_glissement();
                
                AssertEpsilon( resultat , nouveau );
            });
            
            const Vecteur vect_mauvaise_dim{1,2,3};
            const Vecteur vect_bonne_dim{1, 2, 2, 3, 3, 5};
            
            Feature("modification du vecteur d'etat "){
                When("mauvaise dimension"){
                    AssertThrows(std::exception , b.set_etat(vect_mauvaise_dim));
                });
                
                When("bonne dimension"){
                    b.set_etat(vect_bonne_dim);
                    Vecteur resultat = b.etat();
                    AssertEpsilon( resultat , vect_bonne_dim );
                });
            });
            Feature("modification du vecteur deriv_etat"){
                When("mauvaise dimension"){
                    AssertThrows(std::exception , b.set_etat(vect_mauvaise_dim));
                });
                When("bonne dimension"){
                    b.set_derivee_etat(vect_bonne_dim);
                    Vecteur resultat = b.derivee_etat();
                    AssertEpsilon( resultat , vect_bonne_dim );
                    
                });
            });
        });
    });
    
    Feature("on peut l'afficher et la lire"){
        
        //
        // Création d'une boule
        //
        Boule boule_source{1, 2, 3, 4};
        boule_source.set_coef_frot_roulement(5);
        boule_source.set_coef_frot_glissement(6);
        boule_source.set_etat( Vecteur{7, 8, 9, 10, 11, 12} );
        boule_source.set_derivee_etat( Vecteur{13, 14, 15, 16, 17, 18} );
        
        Boule const& boule = boule_source; // on ne veut pas modifier le modèle
        
        //
        // Affichage de la boule
        //
        stringstream flux;
        flux << boule;
        
        //
        // Lecture d'une boule
        //
        When("lecture de la boule"){
            Boule boule_output = Boule::LireDepuis( flux );
            // exception si le format n'est pas reconnu
            
            When("la boule lue est correcte"){
                AssertThat( boule_output , Equals(  boule  ) );
                // erreur si les valeurs sont différentes
            });
            
            //
            // On vérifie que tout le flux à été lu
            //
            When("le flux a totalement été consommé"){
                char c; flux >> c;
                AssertThat(flux.eof() , Is().True() );
                AssertThat(flux.fail(), Is().True() );
            });
        });
    });

    Feature("on peut comparer deux boules"){
        Feature("Quand les deux boules sont exactement egales"){
            
            Boule boule_source{1, 2, 3, 4};
            boule_source.set_coef_frot_roulement(5);
            boule_source.set_coef_frot_glissement(0.1);
            boule_source.set_etat( Vecteur{7, 8, 9, 10, 11, 12} );
            boule_source.set_derivee_etat( Vecteur{13, 14, 15, 16, 17, 18} );
            
            Boule boule_a_comparer = boule_source;
            
            When("la comparaison est stricte"){
                AssertThat(boule_a_comparer, Equals (boule_source))
            });
            
            When("la comparaison est a epsilon pres"){
                bool epsilonEquals = boule_source.epsilon(boule_a_comparer, 10);
                AssertThat(epsilonEquals, Is().True());
            });
        });
        
        Feature("Quand les deux boules sont juste un peu differentes"){
            double cfr = 5;
            double cfg = 0.1;
            double epsilon = std::numeric_limits<double>::epsilon();
            
            Vecteur etat{7, 8, 9, 10, 11, 12};
            Vecteur detat{13, 14, 15, 16, 17, 18};
            Vecteur vepsilon{0, 0, 0, 0, 0, 0 + epsilon};
            
            Boule boule_source    {1, 2, 3, 4};
            Boule boule_a_comparer{1, 2, 3, 4};
            
            boule_source.set_coef_frot_roulement(    cfr);
            boule_a_comparer.set_coef_frot_roulement(cfr + epsilon);
            
            boule_source.set_coef_frot_glissement(    cfg);
            boule_a_comparer.set_coef_frot_glissement(cfg + epsilon);
            
            boule_source.set_etat(     etat );
            boule_a_comparer.set_etat( etat + vepsilon);
            
            boule_a_comparer.set_derivee_etat( detat );
            boule_source.set_derivee_etat(     detat + vepsilon);
            
            When("la comparaison est stricte"){
                AssertThat(boule_source == boule_a_comparer, Is().False());
            });
            When("la comparaison est a epsilon pres"){
                AssertThat(boule_a_comparer.epsilon(boule_source, 10), Is().True() );
            });
        });
        
        Feature("Quand les deux boules sont vraiment differentes"){
            Boule boule_source{1, 2, 3, 4};
            boule_source.set_coef_frot_roulement(5);
            boule_source.set_coef_frot_glissement(0.1);
            boule_source.set_etat( Vecteur{7, 8, 9, 10, 11, 12} );
            boule_source.set_derivee_etat( Vecteur{13, 14, 15, 16, 17, 18} );
            
            Boule boule_a_comparer{5, 6, 7, 8};
            boule_a_comparer.set_coef_frot_roulement(2);
            boule_a_comparer.set_coef_frot_glissement(0.3);
            boule_a_comparer.set_etat( Vecteur{5, 8, 9, 0, 11, 12} );
            boule_a_comparer.set_derivee_etat( Vecteur{13, 13, 15, 16, 17, 18} );
            
            When("la comparaison est stricte"){
               AssertThat(boule_source == boule_a_comparer, Is().False());
            });
            When("la comparaison est a epsilon pres"){
                AssertThat(boule_a_comparer, ! EpsilonEquals (boule_source))
            });
        });
    });
    
    Feature("on peut calculer le point le plus proche"){
        Boule b(1,1,1,1);
        b.set_position({0,0,0});
        struct TestCase{
            Vecteur point_exterieur;
            Vecteur point_de_la_boule;
            Vecteur normal;
            string description;
        };
        auto executer_test = [&](TestCase& t){
            When( t.description.c_str() ){
                auto resultat = b.point_le_plus_proche_de(t.point_exterieur);
                AssertEpsilon(resultat.point, t.point_de_la_boule);
                AssertEpsilon(resultat.normal_au_plan_tangent, t.normal);
            });
        };
        vector<TestCase> tests{
            {{-2,0,0} , {-1,0,0} , {-1,0,0} , "point extérieur à gauche"},
            {{+2,0,0} , {+1,0,0} , {+1,0,0} , "point extérieur à droite"},
            {{0,0,+2} , {0,0,+1} , {0,0,+1} , "point extérieur à devant"},
            {{0,0,0} , {0,0,0} , {0,0,0} , "point au centre"},
            //{{0,0,.5} , {0,0,.5} , {0,0,1} , "point intérieur"},
            // Les points intérieurs ne sont pas censés être utilisés
        };
        for(auto& test : tests){ executer_test(test); }
    });
});


END_BANDIT
BANDIT_RUN_TESTS();
