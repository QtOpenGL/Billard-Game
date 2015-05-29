//  Fichier : billard_general_algo2.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__billard_general_algo2__
#define __Billard__billard_general_algo2__


#include "billard_general.h"

// ===================================================
// Seconde implémentation de l'algorithme d'évolution
//
struct WeakLt;
struct Collision;

template <class DetecteurCollision, class GestionnaireCollision>
class BillardAlgo2_Base : public BillardGeneral{
public:
    using BillardGeneral::BillardGeneral;
    void evoluer(double dt);
private:
    std::map<double, std::set<Collision>, WeakLt> prochaines_collisions_avant_fin_du(double petit_dt);
    bool gerer_collision(std::set<Collision> const& collisions);
};

class DetecteurDeCollision;
class ExecuteurDeCollision;
using BillardAlgo2 = BillardAlgo2_Base<DetecteurDeCollision, ExecuteurDeCollision>;

#include "billard_general_algo2.hpp"

#endif /* defined(__Billard__billard_general_algo2__) */
