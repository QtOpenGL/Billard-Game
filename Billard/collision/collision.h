//  Fichier : collision.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//     Ce fichier implémente les fonctionnalitées principales d'un objet
//     Vis-à-vis d'un autre objet.
//     Il s'agit en fait d'une suite de classes de fonctionnalités
//     que nous avons rassemblées dans un meme fichier.
//

#ifndef Billard_objet_fonctions_collision_objet_h
#define Billard_objet_fonctions_collision_objet_h

#include <utility>
#include <string>
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include "vecteur.h"
class Objet;
class Boule;
class Paroi;
class Brique;
class Sol;
class Trou;
class Integrateur;
class ObstacleImmobile;
class BouleMortelle;

class DetecteurDeCollision {
public:
    struct Collision{
    public:
        double dt_avant_collision;
        bool collision_trouvee;
        
        Collision() = default;
        Collision(std::pair<double, bool> const& p)
        : dt_avant_collision{p.first}, collision_trouvee{p.second} {}
    };
    
    DetecteurDeCollision(double date, double recherche_max, Integrateur& i)
    : date_actuelle{date}, dt_recherche_max{recherche_max}, i{i} {}
    
    Collision detecter   (Objet&, Objet&);
    Collision operator() (Boule&, Boule&);
    
    Collision operator() (Boule&, ObstacleImmobile&);
    Collision operator() (ObstacleImmobile& o, Boule& b){ return this->operator()(b, o); }
    
    Collision operator() (Boule&, Trou& );
    Collision operator() (Trou& a, Boule& b){ return this->operator()(b, a); }
    Collision operator() (Objet&, Objet&){
        std::cout << "Cas de collison imprévu\n";
        return valeur_de_retour = {{0,false}};
    }
private:
    Collision valeur_de_retour{}; //ok car un seul thread
    Integrateur& i;
    double date_actuelle;
    double dt_recherche_max;
};

class DetecteurDeCollision2 { // Continuous Colision Detection
public:
    struct Collision{
    public:
        double dt_avant_collision;
        bool collision_trouvee;
        
        Collision() = default;
        Collision(std::pair<double, bool> const& p)
        : dt_avant_collision{p.first}, collision_trouvee{p.second} {}
    };
    
    DetecteurDeCollision2(double date, double recherche_max, Integrateur& i)
    : date_actuelle{date}, dt_recherche_max{recherche_max}, i{i} {}
    
    Collision detecter   (Objet&, Objet&);
    Collision operator() (Boule&, Boule&);
    
    Collision operator() (Boule&, ObstacleImmobile&);
    Collision operator() (ObstacleImmobile& o, Boule& b){ return this->operator()(b, o); }
    
    Collision operator() (Boule&, Trou& );
    Collision operator() (Trou& a, Boule& b){ return this->operator()(b, a); }
    
    Collision operator() (Objet&, Objet&){
        std::cout << "Cas de collision imprévu\n";
        return valeur_de_retour = {{0,false}};
    }
private:
    Collision valeur_de_retour{}; //ok car un seul thread
    Integrateur& i;
    double date_actuelle;
    double dt_recherche_max;
};


class ExecuteurDeCollision {
public:
    bool executer   (Objet&, Objet&);
    bool operator() (Boule&, Boule&);
    bool operator() (Boule&, ObstacleImmobile&);
    bool operator() (Boule&, Sol   &);
    bool operator() (Boule&, Trou   &);
    bool operator() (Boule&, BouleMortelle&);
    bool operator() (BouleMortelle& a, BouleMortelle& b);
    
    bool operator() (ObstacleImmobile& b, Boule& a){ return this->operator()(a,b); }
    bool operator() (Sol   & b, Boule& a){ return this->operator()(a,b); }
    bool operator() (Trou  & b, Boule& a){ return this->operator()(a,b); }
    bool operator() (BouleMortelle& a, Boule& b){ return this->operator()(b, a); }
    
    bool operator() (Objet&, Objet&){
        std::cout << "Cas de gestion des collision non traité... \n";
        return false;
    }
private:
    bool valeur_de_retour{}; //ok car un seul thread
};

#endif
