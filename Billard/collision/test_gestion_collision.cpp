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
#include "bandit.h"
#include "tests_helper.h"
#include "almost_equal.h"

#include "boule.h"
#include "gestion_collision.h"
#include "paroi.h"
#include "brique.h"
#include "sol.h"
#include "obstacle_immobile.h"

using namespace std;
using namespace BouleFactory;

//
// Here we define some helper functions and structs
//

void AssertEpsilon(Vecteur const& v1, Vecteur const& v2);
void AssertEpsilon(double d1, double d2);

const Vecteur e0{0,0,0};
const Vecteur e1{1,0,0};
const Vecteur e2{0,1,0};
const Vecteur e3{0,0,1};

const Boule boule(Vecteur position = Vecteur{0,0,0}, Vecteur vitesse = Vecteur{0,0,0})
{
    Boule boule_source{1, 1, 1, 1};
    boule_source.set_position(position);
    boule_source.set_vitesse(vitesse);
    
    return boule_source;
}

const Paroi paroi()
{
    return Paroi(e0, e1, e2, 1,1);
}

const Brique brique()
{
    return Brique(e0, e1, e2, e3);
}

const Sol sol()
{
    return Sol(e0, e1, e2, 1, 1);
}


//
// here is where the tests take place
//

BEGIN_BANDIT

Feature("Gestion d'un choc entre deux boules"){
    When("on peut calculer la vitesse relative au point de contact "){
        //Que se passe-t-il s'il n'y a pas contact ??
        
        Boule boule_1 = boule(1, Vecteur{0, 0, 0, 10, 11, 12}, Vecteur{10, 12, 14, 1, 2, 3});
        Boule boule_2 = boule(1, Vecteur{4, 4, 8, 00, 11, 12}, Vecteur{13, 14, 15, 1, 2, 3});
        
        const Vecteur v1 {10, 12, 14};
        const Vecteur v2 {13, 14, 15};
        const Vecteur w1 {1, 2, 3};
        const Vecteur w2 {1, 2, 3};
        const double  vit_normal = produit_scalaire({0,0,0}, {4,4,8});
        const Vecteur normal {-4/9, -4/9, -8/9};
        
        Vecteur v_r_voule = (v1-v2) + (vit_normal * normal) + produit_vectoriel(normal, w1 + w2);
        Vecteur v_r = calcul_v_relative_point_choc(boule_1, boule_2, normal, vit_normal);
        
        AssertEpsilon(v_r_voule, v_r);
        
    });
    Feature("on peut calculer delta_v_delta_w"){
        When("la position de la boule 1 = position de la boule 2"){
            Boule boule_1{1, 2, 3, 4};
            Boule boule_2{1, 6, 7, 8};
            boule_1.set_etat( Vecteur{2, 2, 8, 10, 11, 12} );
            boule_2.set_etat( Vecteur{2, 2, 8, 0, 11, 12} );
            
            pair<Vecteur, Vecteur> voulu = {{0, 0, 0},{ 0, 0, 0}};
            pair<Vecteur, Vecteur> v = calcul_deltaV_deltaW(boule_1, boule_2);
            
            AssertThat(voulu, Equals(v));
        });
        
    });
    Feature("Gestion d'un collision entre deux boules de même masse"){
        Vecteur pos1{ 0,0,0 , 0,0,0 } , pos2{ 2,0,0 , 0,0,0 };
        
        Feature("lorsque v1 = - v2"){
            Vecteur vit1{ +2,0,0 , 0,0,0 } , vit2 = -vit1;
            
            Boule const b1_original = boule(1, pos1, vit1);
            Boule const b2_original = boule(1, pos2, vit2);
            
            
            Describe("la gestion du choc")
            {
                Boule   b1  = b1_original , b2  = b2_original;
                Vecteur ov1 = b1_original.get_vitesse(), ov2 = b2_original.get_vitesse();
                
                It("la mise à jour ne lance pas d'exception")
                {
                    mise_a_jour_apres_collision(b1, b2);
                });
                
                Vecteur v1  = b1.get_vitesse() , v2  = b2.get_vitesse();
                
                It("les nouvelles vitesses sont opposées")
                {
                    double ps = produit_scalaire(v1, v2);
                    double ops = produit_scalaire(ov1, ov2);
                    
                    AssertThat( ps * ops >= 0, Is().True() );
                });
                It("la vitesse de la première boule est correctement mise à jour")
                {
                    double ps = produit_scalaire(v1, ov1);
                    AssertThat( ps , Is().LessThan(0) );
                });
                It("la vitesse de la seconde boule est correctement mise à jour")
                {
                    double ps = produit_scalaire(v2, ov2);
                    AssertThat( ps , Is().LessThan(0) );
                });
                It("la quantité de mouvement s'est conservée")
                {
                    double ec1 = b1_original.energie_cinetique() + b2_original.energie_cinetique();
                    double ec2 = b1.energie_cinetique() + b2.energie_cinetique();
                    AssertThat(ec2, EpsilonEquals(ec1));
                });
                It("le choc est élastique")
                {
                    AssertThat(-b1.get_masse() * (v1 - ov1), EpsilonEquals(   b2.get_masse() * (v2 - ov2)   ));
                });
            });
        });
    });
});

Feature("gestion choc boule / boule :")
{
    When("une boule est a côter d'une boule ")
    {
        Boule b1 = boule(e0);
        Boule b2 = boule(2*e1);
        
        mise_a_jour_apres_collision(b1, b2);
        
        AssertThat(b1.get_position(), Equals(e0));
        AssertThat(b2.get_position(), Equals(2*e1));
        
        AssertEpsilon(b1.get_vitesse(), e0);
        AssertEpsilon(b2.get_vitesse(), e0);
    });
    
    When("les boules vont dans le même sens avec des vitesses différentes")
    {
        Boule b1 = boule(e0, 2*e1);
        Boule b2 = boule(2*e1, e1);
    });
    
    When("les boules vont dans le même sens avec des vitesse identique")
    {
        Boule b1 = boule(e0,e1);
        Boule b2 = boule(2*e1, e1);
        
        mise_a_jour_apres_collision(b1, b2);
        
        AssertEpsilon(b1.get_vitesse(), e1);
        AssertEpsilon(b2.get_vitesse(), e1);
        
    });
    
    When("une boule rentre dans une boule immoblie")
    {
        Boule b1 = boule(e0,3*e1);
        Boule b2 = boule(2*e1);
        
        mise_a_jour_apres_collision(b1, b2);
        
        AssertThat(b2.get_vitesse()[0] > 0.0, Is().True());
        
    });

});


Feature("gestion choc boule / paroi : ")
{
    Paroi p = paroi();
    
    Before_each(){
        p = paroi();
    });
    
    When("la mise à jour ne lance pas d'exception")
    {
        Boule b = boule(e3);
        
        mise_a_jour_apres_collision(b, p);
    });
    
        
    When ("une boule avance vers une paroi")
    {
        Boule b = boule(e3);
        b.set_vitesse(-2*e3);
        mise_a_jour_apres_collision(b, p);
        
        AssertThat(b.get_vitesse()[2] > 0.0, Is().True());
    });
    
    When("une boule immobile est à côté d'une paroi ")
    {
        Boule b = boule(e3);
        
        try{
            mise_a_jour_apres_collision(b, p);
        }catch(exception& e){
            cout << endl << e.what() << endl;
            throw;
        }
        AssertEpsilon(b.get_vitesse(),  e0);
        AssertEpsilon(b.get_position(), e3);        
    });
    
});


Feature("gestion choc boule / brique : ")
{
    Brique br = brique();
    
    When("une boule est à coté d'une brique ")
    {
        Boule b = boule(2*e3);
        
        mise_a_jour_apres_collision(b, br);
        
        AssertEpsilon(b.get_position(), 2*e3);
        AssertEpsilon(b.get_vitesse(), e0);
        
    });
    
    
    
    

});

Feature("gestion choc boule / sol : ")
{
    Sol s = sol();
    
    When("la boule est posée sur le sol ")
    {
        Boule b = boule(e3);
        b.set_vitesse(e0);
        
        mise_a_jour_apres_collision(b, s);
        
        AssertEpsilon(b.get_vitesse(), e0);
        AssertEpsilon(b.get_position(), e3);
    });
    
    When("une boule roule")
    {
        Boule b = boule(e3);
        b.set_vitesse(e2);
        mise_a_jour_apres_collision(b, s);
        
        AssertEpsilon(b.get_vitesse(), e2);
    });
});
//*/
    

END_BANDIT
BANDIT_RUN_TESTS();


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
