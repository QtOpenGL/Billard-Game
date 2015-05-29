//  Fichier : collision.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//

#include <memory>
#include <iostream>

#include "collision.h"

#include "objet_visiteur.h"
#include "objet.h"
#include "boule.h"
#include "boule_de_couleur.h"
#include "boule_mortelle.h"
#include "paroi.h"
#include "brique.h"
#include "sol.h"
#include "trou.h"

#include "multiple_dispatch.h"
#include "gestion_collision.h"
#include "detection_collision.h"

using namespace std;


//==============================
// Detecteur
//==============================

DetecteurDeCollision::Collision DetecteurDeCollision::
detecter(Objet& lhs, Objet& rhs)
{
    dispatch::execute(*this, ObjetVisiteur::FamilleObjet(lhs), ObjetVisiteur::FamilleObjet(rhs));
    return valeur_de_retour;
}

DetecteurDeCollision::Collision DetecteurDeCollision::
operator() (Boule& lhs, Boule& rhs)
{
    auto collision = dt_jusque_collision(lhs, rhs, date_actuelle, dt_recherche_max, i);
    return valeur_de_retour = Collision{collision};
}

DetecteurDeCollision::Collision DetecteurDeCollision::
operator() (Boule& lhs, ObstacleImmobile& rhs)
{
    auto collision = dt_jusque_collision(lhs, rhs, date_actuelle, dt_recherche_max, i);
    return valeur_de_retour = Collision{collision};
}

DetecteurDeCollision::Collision DetecteurDeCollision::
operator()(Boule& lhs, Trou& rhs){
    auto collision = dt_jusque_collision(lhs, rhs, date_actuelle, dt_recherche_max, i);
    return valeur_de_retour = Collision{collision};
}

//==============================
// Version 2
//==============================
DetecteurDeCollision2::Collision DetecteurDeCollision2::
detecter(Objet& lhs, Objet& rhs)
{
    dispatch::execute(*this, ObjetVisiteur::FamilleObjet(lhs), ObjetVisiteur::FamilleObjet(rhs));
    return valeur_de_retour;
}

DetecteurDeCollision2::Collision DetecteurDeCollision2::
operator() (Boule& lhs, Boule& rhs)
{
    auto collision = dt_jusque_collision(lhs, rhs, date_actuelle, dt_recherche_max, i);
    return valeur_de_retour = Collision{collision};
}

DetecteurDeCollision2::Collision DetecteurDeCollision2::
operator() (Boule& lhs, ObstacleImmobile& rhs)
{
    auto collision = dt_jusque_collision2(lhs, rhs, date_actuelle, dt_recherche_max, i);
    return valeur_de_retour = Collision{collision};
}

DetecteurDeCollision2::Collision DetecteurDeCollision2::
operator() (Boule& lhs, Trou& rhs)
{
    return valeur_de_retour;
}


//==============================
// Executeur
//==============================

bool ExecuteurDeCollision::
executer (Objet& lhs, Objet& rhs)
{
    dispatch::execute(*this, ObjetVisiteur::FamilleObjet(lhs), ObjetVisiteur::FamilleObjet(rhs));
    return valeur_de_retour;
}

bool ExecuteurDeCollision::
operator() (Boule& lhs, Boule& rhs)
{
    valeur_de_retour = mise_a_jour_apres_collision(lhs, rhs);
    return valeur_de_retour;
}

bool ExecuteurDeCollision::
operator() (Boule& lhs, ObstacleImmobile& rhs)
{
    valeur_de_retour = mise_a_jour_apres_collision(lhs, rhs);
    return valeur_de_retour;
}
bool ExecuteurDeCollision::
operator() (Boule& lhs, Sol   & rhs)
{
    valeur_de_retour = mise_a_jour_apres_collision(lhs, rhs);
    return valeur_de_retour;
}
bool ExecuteurDeCollision::
operator() (Boule& a, Trou& b)
{
    a.set_vie(false);
    return valeur_de_retour = false;
}

bool ExecuteurDeCollision::
operator() (Boule& b, BouleMortelle& bm)
{
    auto retour = this->operator()(static_cast<Boule&>(bm), b);
    b.set_vie(false);
    return valeur_de_retour = retour;
}
bool ExecuteurDeCollision::
operator() (BouleMortelle& a, BouleMortelle& b)
{
    auto retour = this->operator()(static_cast<Boule&>(a), static_cast<Boule&>(b));
    a.set_vie(false);
    b.set_vie(false);
    return valeur_de_retour = retour;
}


