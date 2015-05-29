//  Fichier : billard_general.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__billard_general__
#define __Billard__billard_general__

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <stdexcept>

#include "billard.h"
class Boule;
class Objet;
class Sol;
class ObstacleImmobile;
class Integrateur;

class ObjetNulException : public std::runtime_error{
public: ObjetNulException()
    : std::runtime_error{"Les objets ne peuvent être nullptr pour la classe BillardGeneral"}
    {}
};

class IntegrateurNulException : public std::runtime_error{
public: IntegrateurNulException()
    : std::runtime_error{"L'integrateur ne peuvent être nullptr pour la classe BillardGeneral"}
    {}
};

//
// Préconditions :
// a. Les objets (Integrables et Immobiles) manipulés ne sont jamais nullptr
// b. L'integrateur n'est jamais nullptr

class BillardGeneral : public Billard {
public:
    BillardGeneral(std::unique_ptr<Sol> sol, std::unique_ptr<Integrateur> integrateur);
    BillardGeneral(BillardGeneral const& autre) = delete;
    BillardGeneral& operator=(BillardGeneral const& autre) = delete;
    
    BillardGeneral(BillardGeneral&& autre);
    BillardGeneral& operator=(BillardGeneral&& autre);
    virtual ~BillardGeneral() {}
    
    virtual void evoluer(double dt) = 0;
    void se_dessiner(Viewer& viewer) override ;
    
    // Ajouter un objet
    void ajouter(std::unique_ptr<ObstacleImmobile> o);
    void ajouter(std::unique_ptr<Boule>  o);
    
    // Modifier l'intégrateur
    void integrateur(std::unique_ptr<Integrateur> nouveau);
    std::unique_ptr<Integrateur> const& integrateur() const { return integrateur_; }
    
    double date_actuelle() const { return date_actuelle_; }
    
    static constexpr double SEUIL_VITESSE_LINEAIRE  = 1e-2;
    static constexpr double SEUIL_VITESSE_ANGULAIRE = 1e-1;
    static constexpr double SEUIL_FORCE = 1e-10;
protected:
    std::vector< std::unique_ptr<Objet> > memory_pool_;
    std::vector< ObstacleImmobile* > obstacles_;
    std::vector< Boule*  > objets_;
    size_t index_du_sol_dans_memory_pool_;
    Sol* sol_;
protected:
    void faire_avancer_le_temps(double dt_integrateur);
    void supprimer_les_objets_morts();
    
private:
    std::unique_ptr<Integrateur> integrateur_;
    double date_actuelle_ = 0 /*s*/;
    //double dt_integrateur_ = 0.01 /*s*/;
};
class DummyBillardGeneral : public BillardGeneral{
public:
    using BillardGeneral::BillardGeneral;
    void evoluer(double dt) override {(void) dt;}
};


// ===================================================
// Premiere implémentation de l'algorithme d'évolution
//
struct ProchaineCollision; //billard_general.cpp

class BillardAlgo1 : public BillardGeneral{
public:
    using BillardGeneral::BillardGeneral;
    void evoluer(double dt);
private:
    ProchaineCollision prochaine_collision_avant_fin_du(double petit_dt);
    bool gerer_collision(ProchaineCollision&& rvalue_collision); //&& pour rvalue
};


// ===================================================
// Seconde implémentation de l'algorithme d'évolution
//
struct WeakLt;
struct Collision;

class BillardAlgo2 : public BillardGeneral{
public:
    using BillardGeneral::BillardGeneral;
    void evoluer(double dt) override;
private:
    std::map<double, std::set<Collision>, WeakLt> prochaines_collisions_avant_fin_du(double petit_dt);
    bool gerer_collision(std::set<Collision> const& collisions);
    void faire_avancer_le_temps_securise(double dt);
};



#endif /* defined(__Billard__billard_general__) */
