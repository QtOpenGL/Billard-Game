//  Fichier : test_billard_general.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : test pour la classe BillardGeneral
//


#include <string>
#include <iostream>
#include <memory>
#include <iomanip>
#include <stdexcept>
#include "bandit.h"
#include "tests_helper.h"

#include "billard_general.h"
#include "coefficients_physiques.h"
#include "billard_builder.h"

#include "integrateur.h"

#include "objet_mobile.h"
#include "obstacle_immobile.h"
#include "boule.h"
#include "sol.h"

using namespace std;
using namespace BouleFactory;
//
// Nous définissons d'abord des dummy classes
//

class DummyIntegrateur : public Integrateur{
public:
    void integre(Integrable& objet, double temps, double dt) const override {}
    static unique_ptr<Integrateur> GET()
    {
        return unique_ptr<Integrateur>{ new DummyIntegrateur{} };
    }
};
class DummyObjet : public ObjetMobile{
public:
    virtual Vecteur get_acceleration(double temps /*s*/) const {return {0,0,0};};
    virtual Vecteur get_acc_angulaire(double temps /*s*/) const {return {0,0,0};};
    virtual double  get_masse()  const {return {};} ;
    virtual double  get_volume() const {return {};} ;
    virtual double  get_coef_frot_restitution() const {return {};};
    virtual double  get_coef_frot_propre()      const {return {};};
    virtual void accept(ObjetVisiteur& visiteur) {}
    virtual void mise_a_jour_resultante_des_forces() {}
    virtual PointEtPlan point_le_plus_proche_de(Vecteur const& v) const;
    static unique_ptr<ObjetMobile> GET()
    {
        return unique_ptr<ObjetMobile>{ new DummyObjet{} };
    }
};
class DummyObstacleImmobile : ObstacleImmobile{
public:
    DummyObstacleImmobile() : ObstacleImmobile({0,0,0}, 0,0,0) {}
    
    Vecteur get_position() const { return {}; }
    double  get_volume()   const { return {}; }
    double  get_coef_frot_restitution() const { return {}; }
    double  get_coef_frot_propre()      const { return {}; }
    void accept(ObjetVisiteur& visiteur) {}
    PointEtPlan point_le_plus_proche_de(Vecteur const& boule) const { return {{0,0,0},{}}; }
    
    static unique_ptr<ObstacleImmobile> GET()
    {
        return unique_ptr<ObstacleImmobile>{ new DummyObstacleImmobile{} };
    }
};
template <class T, class... Args>
unique_ptr<T> make(Args&&... args){
    return unique_ptr<T>{ new T(forward<Args>(args)...) };
}

unique_ptr<Boule> dummy_boule(){
    return make<Boule>(1./*cm*/, 1./*g*/, 1., 1.);
}
unique_ptr<Sol> dummy_sol(){
    return make<Sol>(Vecteur{0,0,0}, Vecteur{1,0,0}, Vecteur{0,2,0},1,1,1,1);
}
unique_ptr<ObstacleImmobile> dummy_obstacle()
{
    return DummyObstacleImmobile::GET();
}
unique_ptr<Boule> get_boule(Vecteur position = {0,0,0}){
    return make<Boule>(  BouleFactory::boule(1., concatenation(position, {0,0,0}))  );
}

//
// Tests pour la classe DummyBillardGeneral
//
BEGIN_BANDIT

Describe("un billard"){
    
    It("peut être créé avec un integrateur non nul")
    {
        DummyBillardGeneral billard{dummy_sol(), DummyIntegrateur::GET()};
        
        auto b = billard3x3AvecTrousSansBrique<DummyBillardGeneral>();
        
        
    });
    It("ne peut pas être créé avec un integrateur nul")
    {
        unique_ptr<Integrateur> integrateur{nullptr};
        AssertThrows(IntegrateurNulException, DummyBillardGeneral(dummy_sol(), move(integrateur)) );
    });
    
    //================================
    DummyBillardGeneral billard{ dummy_sol(), DummyIntegrateur::GET() };
    
    It("peut ajouter un nouvel objet")
    {
        billard.ajouter( dummy_boule() );
    });
    It("peut ajouter un nouvel obstacle")
    {
        billard.ajouter( DummyObstacleImmobile::GET() );
    });
    
    It("refuse les objets nullptr")
    {
        AssertThrows(  ObjetNulException, billard.ajouter( unique_ptr<Boule>{} )  );
    });
    It("refuse les obstacles nullptr")
    {
        AssertThrows(  ObjetNulException, billard.ajouter( unique_ptr<ObstacleImmobile>{})  );
    });
});

/*
Describe("la gestion des objets"){
    class SpyObjet : public Boule{
    public:
        SpyObjet(bool* temoin) : Boule(1,1,1,1), temoin{temoin} {}
        bool* temoin;
        ~SpyObjet(){ *temoin = true; }
    };
    
     It("peut supprimer un objet qui n'est plus en vie"){
        DummyBillardGeneral billard{ dummy_sol(), DummyIntegrateur::GET() };
        
        bool objet_bien_supprime = false;
        SpyObjet* spy = new SpyObjet{&objet_bien_supprime};
        spy->set_vie(false);
        
        billard.ajouter( unique_ptr<Boule>{spy} );
        AssertThat(objet_bien_supprime, Is().False());
        
        billard.evoluer(1);
        AssertThat(objet_bien_supprime, Is().True());
    });
});
 */

/*
Describe("L'algorithme d'évolution "){
    Vecteur const O  = {0,0,0};
    Vecteur const e1 = {1,0,0};
    Vecteur const e2 = {0,1,0};
    Vecteur const e3 = {0,0,1};
    
    auto get_billard = [&](){
        return DummyBillardGeneral{ make<Sol>(O, 10*e1, 10*e2, 1, 1), make<Integrateur_Euler>() };
    };
    
    Feature("peut faire avancer une boule "){
        DummyBillardGeneral billard = get_billard();
        double dt = 1;
        
        auto ajouter_nouvelle_boule = [&](double x, double y){
            auto b = get_boule();
            auto ptr = b.get();
            b->set_position(O + x*e1 + y*e2 + b->get_rayon()*e3);
            billard.ajouter(std::move(b));
            return ptr;
        };
        
        Before_each(){
            billard = get_billard();
        });
        
        When("qui est immobile "){
            auto ptr = ajouter_nouvelle_boule(0,0);
            
            Vecteur position_originale = ptr->get_position();
            
            billard.evoluer(1);
            
            AssertThat(ptr->get_position(), EpsilonEquals( position_originale ));
        });
        
        When("qui possède une vitesse constante "){
            
            auto ptr = ajouter_nouvelle_boule(0,0);
            ptr->set_vitesse(e1);
            Vecteur position_originale = ptr->get_position();
            
            billard.evoluer(dt); //mise à jour de la dérivée de l'état
            
            AssertThat(ptr->get_position(), EpsilonEquals( position_originale + dt * e1));
        });
        When("qui possède une vitesse constante et un poids"){
            
            auto ptr = ajouter_nouvelle_boule(0,0);
            ptr->set_vitesse(e1);
            ptr->set_resultante_des_forces( {0,0, ptr->get_masse()*9.81} );
            Vecteur position_originale = ptr->get_position();
            
            billard.evoluer(dt); //mise à jour de la dérivée de l'état
            Vecteur position_attendue = position_originale + (dt) * e1;
            
            AssertThat(ptr->get_position(), EpsilonEquals( position_attendue ));
            
            for(int i = 0 ; i < 10; ++ i){
                billard.evoluer(dt); //mise à jour de la dérivée de l'état
                position_attendue += dt * e1;
            }
            
            AssertThat(ptr->get_position(), EpsilonEquals( position_attendue ));
        });
            When("qui n'est pas au contact du sol"){
            
            auto ptr = ajouter_nouvelle_boule(0,0);
            ptr->set_vitesse(e1);
            ptr->set_position(ptr->get_position() + e3);
            Vecteur position_originale = ptr->get_position();
            
            billard.evoluer(dt); //mise à jour de la dérivée de l'état
            billard.evoluer(dt);
            
            AssertThat(ptr->get_position(), EpsilonEquals( position_originale + 2 * dt * e1));
            });
    });
    
    Feature("peut faire avancer deux boules "){
       
        DummyBillardGeneral billard = get_billard();
        double dt = 0.1;
        
        auto ajouter_nouvelle_boule = [&](double x, double y){
            auto b = get_boule();
            auto ptr = b.get();
            b->set_position(O + x*e1 + y*e2 + b->get_rayon()*e3);
            billard.ajouter(std::move(b));
            return ptr;
        };
        
        Before_each(){
            billard = get_billard();
        });
        
        When("qui est immobile "){
            auto ptr = ajouter_nouvelle_boule(3,3);
            Vecteur position_originale = ptr->get_position();
            ajouter_nouvelle_boule(9,9);
            
            billard.evoluer(dt);
            AssertThat(ptr->get_position(), EpsilonEquals( position_originale ));
        });
        
        When("Le billard peut gérer une collision sans qu'aucune erreur ne soit lancée"){
            Boule b1 = Boule::Nouvelle(1, 1, 1, 1,{10,10,1 , 10,0,0}, {-10,0,0 , 0,0,0});
            Boule b2 = Boule::Nouvelle(1, 1, 1, 1,{ 0,10,1 ,  0,10,0},{+10,0,0 , 0,0,0});
            billard.ajouter(std::unique_ptr<Boule>{new Boule{b1}});
            billard.ajouter(std::unique_ptr<Boule>{new Boule{b2}});
            
            for(int i = 0; i < 100000; ++i){
                billard.evoluer(1.0/ 1000.0);
            }
        });
        
        When("Nouveau cas de test"){
            DummyBillardGeneral billard{make<Sol>(Vecteur{0,0,0},Vecteur{100,0,0},Vecteur{0,200,0}), std::unique_ptr<Integrateur>(new Integrateur_Euler)};
            
            Boule* b1 = new Boule{Boule::Nouvelle(1, 1, 1, 1,{ 5,20, 5 , 1, 1, 1}, { 0, 0, 10   , 0,0,0})};
            Boule* b2 = new Boule{Boule::Nouvelle(1, 1, 1, 1,{ 5,20, 10 , 1, 1, 1}, { 0, 0,  0   , 0,0,0})};
            
            billard.ajouter(std::unique_ptr<Boule>{b1});
            billard.ajouter(std::unique_ptr<Boule>{b2});
            
            for(int i = 0; i < 100000; ++i){
                auto p1 = b1->get_position();
                auto p2 = b2->get_position();
                
                billard.evoluer(1.0/ 1000.0);
                
                AssertThat(p1, Is().Not().EqualTo( b1->get_position() ));
                AssertThat(p2, Is().Not().EqualTo( b2->get_position() ));
            }
        });
        When("last test case..."){
            DummyBillardGeneral billard{make<Sol>(Vecteur{0,0,0},Vecteur{51,0,0},Vecteur{0,51,0},1,1,1,1), std::unique_ptr<Integrateur>(new Integrateur_Euler)};
            
            for(double x = 30; x <= 33; x += 5){
                for(double y = 30; y <= 33; y += 5){
                    Boule b = Boule::Nouvelle(1, 100000, 1, 1,{x,y,1 , 0,0,0}, {0,0,0 , 0,0,0});
                    billard.ajouter(  std::unique_ptr<Boule>{new Boule(b)}  );
                }
            }
            
            Boule b = Boule::Nouvelle(1, 100000, 1, 1,{20,20,1 , 0,0,0}, {10,10,0 , 0,0,0});
            billard.ajouter(  std::unique_ptr<Boule>{new Boule(b)}  );
            
            billard.evoluer(1);
 
        });
    });
});
//*/

END_BANDIT
BANDIT_RUN_TESTS();
