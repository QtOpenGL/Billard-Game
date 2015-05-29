//  Fichier : vecteur_tests.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Tests pour la class Vecteur mathématique
//

#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
using namespace std;

#include "bandit.h"
#include "vecteur.h"
#include "tests_helper.h"
//
// Les ensembles de tests :
//
// Describe (string) { ... }); = description
// Feature (string) { ... });  = fonctionnalité ou fonction
//
// Les tests :
//
// When (string) { ... }); = quand
// It (string)   { ... }); = il
//
// Pour vérifier :
//
// AssertThat( expression , condition );
// AssertThrows( std::exception,  code );
//
// où condition peut etre :
//
// EpsilonEquals( .... )
// Is().True()
// Is().False()


BEGIN_BANDIT

Describe("un vecteur")
{
    
    Feature("il peut etre créé")
    {
        Feature("initializer_liste")
        {
            When("elle est vide"){
                AssertThrows(std::length_error, Vecteur{} );
            });
            
            When("elle n'est pas vide"){
                Vecteur v {1, 2, 3, 4} ;
            });
        });
        Feature("creation avec une taille")
        {
            When("la taille vaut 0"){
                AssertThrows(std::length_error, Vecteur::DeTaille(0) );
            });
            When("la taille ne vaut pas 0"){
                Vecteur v = Vecteur::DeTaille(3);
                Vecteur resultat = {0, 0, 0};
                AssertThat( epsilon(v, resultat), Is().True()  );
            });
        });
    });
    
    Feature("il connait sa taille")
    {
        When("il n'est pas vide"){
            const size_t taille = 30;
            Vecteur v = Vecteur::DeTaille( taille );
            AssertThat( v.size() , Equals( taille ) );
        });
        When("il est construit avec une liste de valeurs"){
            Vecteur v{1, 2, 3};
            AssertThat( v.size() , Equals( 3 ));
        });
        When("on lui ajoute une nouvelle dimension"){
            Vecteur origin{1, 2, 3};
            AssertThat( origin.size() , Equals( 3 ));
            
            origin.push_back(0);
            AssertThat( origin.size(), Equals(4));
        });
    });
    
    Feature("on peut acceder a ses coordonnees")
    {
        Vecteur vecteur{ 0, 1, 2, 3 };
        
        When("il n'est pas constant"){
            for(int i = 0; i < vecteur.size(); ++i){
                AssertThat(vecteur[i], Equals(i));
            }
        });
        When("il est constant"){
            Vecteur const& vecteur_constant = vecteur;
            for(int i = 0; i < vecteur_constant.size(); ++i){
                AssertThat(vecteur_constant[i], Equals(i));
            }
        });
    });
    
    When("on lui ajoute une dimension"){
        
        Vecteur depart {1, 2, 3};
        Vecteur attendu{1, 2, 3, 4};
        double last_element(4);
        
        depart.push_back(last_element);
        
        AssertThat(depart, EpsilonEquals(attendu));
    });
    
    Feature("on peut lui ajouter un vecteur existant")
    {
        Vecteur initial_lhs{ 1, 2, 3};
        Vecteur lhs = initial_lhs;
        
        Before_each(){
            lhs = initial_lhs;
        });
        When("le second vecteur est nul"){
            Vecteur rhs{0, 0, 0};
            lhs += rhs;
            AssertThat( lhs , EpsilonEquals( initial_lhs ));
        });
        When("le second vecteur n'est pas nul"){
            Vecteur rhs{3, 2, 1};
            Vecteur resultat{4, 4, 4};
            lhs += rhs;
            AssertThat( lhs, EpsilonEquals( resultat ))
        });
        When("le second vecteur n'a pas la meme dimension"){
            Vecteur rhs = Vecteur::DeTaille( lhs.size() + 1 );
            AssertThrows(std::exception, lhs += rhs);
        });
    });
    
    Feature("on peut lui soustraire un vecteur existant")
    {
        Vecteur initial_lhs{ 1, 2, 3};
        Vecteur lhs = initial_lhs;
        
        Before_each(){
            lhs = initial_lhs;
        });
        When("le second vecteur est nul"){
            Vecteur rhs{0,0,0};
            lhs -= rhs;
            AssertThat(lhs, EpsilonEquals(initial_lhs));
        });
        When("le second vecteur n'est pas nul"){
            Vecteur rhs{3, 2, 1};
            Vecteur resultat{-2, 0, 2};
            lhs -= rhs;
            AssertThat( lhs, EpsilonEquals( resultat ))
        });
        When("le second vecteur n'a pas la meme dimension"){
            Vecteur rhs = Vecteur::DeTaille( lhs.size() + 1 );
            AssertThrows(std::exception, lhs -= rhs);
        });
    });
    
    Feature("on peut le multiplier par un scalaire ")
    {
        Vecteur initialise_lhs{ 1, 2, 3};
        Vecteur lhs = initialise_lhs;
        
        Before_each(){
            lhs = initialise_lhs;
        });
        
        When("le scalaire est 0 "){
            lhs *= 0;
            AssertThat(lhs , EpsilonEquals( Vecteur{0,0,0} ));
        });
        When("le scalaire est 1 "){
            lhs *= 1;
            AssertThat(lhs, EpsilonEquals(initialise_lhs));
        });
        When("le scalaire est -1"){
            lhs *= -1;
            Vecteur rhs{ -1, -2, -3};
            AssertThat(lhs, EpsilonEquals(rhs));
        });
        When("le scalaire est arbitraire"){
            lhs *= 6;
            Vecteur resultat{ 6, 12, 18};
            AssertThat(lhs, EpsilonEquals(resultat));
        });
    });
    
    Feature("on peut le diviser par un scalaire")
    {
        Vecteur initialise_lhs{ 2, 4, 6};
        Vecteur lhs = initialise_lhs ;
        
        Before_each(){
            lhs = initialise_lhs;
        });
        
        When("le scalaire est 0 "){
           AssertThrows( std::exception, lhs /= 0 );
        });
        When("le scalaire est 1"){
            lhs /= 1;
            AssertThat(lhs, EpsilonEquals(initialise_lhs));
        });
        When("le scalaire est quelconque "){
            lhs /= 2;
            Vecteur expected{1, 2, 3};
            AssertThat(lhs, EpsilonEquals(expected));
        });
        When("le scalaire est negatif "){
            lhs /= -2;
            Vecteur expected{-1, -2, -3};
            AssertThat(lhs, EpsilonEquals(expected));
        });
    });
    
    Feature("on peut calculer son opposé" )
    {
        When ("le vecteur est nul"){
            Vecteur lhs{0,0,0};
            Vecteur resultat{0,0,0};
            
            AssertThat( -lhs, EpsilonEquals(resultat));
        });
        When ("Le vecteur est quelconque "){
            Vecteur lhs{4,3,2};
            Vecteur resultat{-4,-3,-2};
            
            AssertThat( -lhs, EpsilonEquals(resultat));
        });
    });
    
    Feature("on peut tester l egalité avec un autre vecteur "){
        Vecteur lhs {1, 2, 3};
        
        When("les deux vecteur sont egaux "){
            Vecteur rhs{1, 2, 3};
            AssertThat(lhs == rhs , Is().True());
        });
        When ("les deux vecteur ne sont pas egaux"){
            Vecteur rhs {2, 3, 4};
            AssertThat(lhs == rhs , Is().False());
        });
    });
    
    Feature("on peut tester la difference avec un autre vecteur" ){
        Vecteur lhs {1, 2, 3};
        
        When("les deux vecteur sont egaux "){
            Vecteur rhs{1, 2, 3};
            AssertThat(lhs != rhs , Is().False());
        });
        When ("les deux vecteur ne sont pas egaux"){
            Vecteur rhs {2, 3, 4};
            AssertThat(lhs != rhs , Is().True());
        });
    });
    
    Feature("on peut tester si deux vecteur sont a epsilon proche" ){
#warning TODO Epsilon Proche
    });
    
    Feature("on peut calculer le produit scalaire"){
        Vecteur lhs {1, 2, 3};
        
        When("l'un des deux est nul"){
            Vecteur rhs {0, 0, 0};
            double resulat(0);
            AssertThat(produit_scalaire(lhs, rhs), EpsilonEquals(resulat));

        });
        When("ils sont orthogonaux"){
            Vecteur e1 { 1, 0, 0};
            Vecteur e2 { 0, 1, 0};
            Vecteur e3 { 0, 0, 1};
            
            AssertThat(produit_scalaire(e1, e2), EpsilonEquals( 0. ));
            AssertThat(produit_scalaire(e2, e3), EpsilonEquals( 0. ));
            AssertThat(produit_scalaire(e3, e1), EpsilonEquals( 0. ));
        });
        When("ils sont quelconques"){
            Vecteur rhs {2, 3, 4};
            double resulat(20);
            AssertThat(produit_scalaire(lhs, rhs), EpsilonEquals(resulat));
        });
    });

    Feature ("on peut calculer la norme_carree"){
        
        When("le vecteur est nul"){
            Vecteur lhs {0,0,0};
            double resultat(0);
            AssertThat(norme_carree(lhs), EpsilonEquals(resultat));
        });
        When("le vecteur est quelconque"){
            Vecteur lhs {3, 4, 5};
            double resultat (50);
            AssertThat(norme_carree(lhs), EpsilonEquals(resultat));
        });
    });
    
    Feature("on peut calculer la norme"){
        
        When("le vecteur est nul"){
            Vecteur lhs {0,0,0};
            double resultat(0);
            AssertThat(norme(lhs), EpsilonEquals(resultat));
        });
        When("le vecteur est quelconque"){
            Vecteur lhs {3, 4, 5};
            double resultat (sqrt(50));
            AssertThat(norme(lhs), EpsilonEquals(resultat));
        });
    });
    
    Feature("on peut calculer le produit vectoriel"){
        
        When("ils sont quelconques"){
            Vecteur lhs      { 1,  2,  3};
            Vecteur rhs      { 2,  3,  4};
            Vecteur resultat {-1 , 2, -1};
            AssertThat(produit_vectoriel(lhs, rhs), EpsilonEquals(resultat));
        });
        When("ils sont colinéaires"){
            Vecteur lhs      { 1,  2,  3};
            Vecteur rhs      = 2 * lhs;
            Vecteur resultat { 0,  0,  0};
            AssertThat(produit_vectoriel(lhs, rhs), EpsilonEquals(resultat));
        });
    });
    
    Feature("on peut concatener deux vecteur"){
        Vecteur debut    {1, 2, 3};
        
        When("ils sont de meme taille"){
            Vecteur fin{4, 5, 6};
            Vecteur resultat_voulu {1, 2, 3, 4, 5, 6};
            
            Vecteur resultat = concatenation(debut, fin);
            
            AssertThat(resultat, Equals(resultat_voulu));
        });
        When("ils sont de taille différentes"){
            Vecteur fin{4, 5   };
            Vecteur resultat_voulu {1, 2, 3, 4, 5};
            
            Vecteur resultat = concatenation(debut, fin);
            
            AssertThat(resultat, Equals(resultat_voulu));
        });
    });
});

Feature("on peut afficher un vecteur dans un flux"){
    When("avec l'operateur <<"){
        Vecteur v{1, 2, 3};
        string expected = "1, 2, 3";
        stringstream flux;
        flux << v;
        string resultat = flux.str();
        
        AssertThat(resultat, Equals(expected));
    });
    
});
END_BANDIT

BANDIT_RUN_TESTS();
