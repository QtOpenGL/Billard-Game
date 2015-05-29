//  Fichier : object_viewer.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#include "multiple_dispatch.h"
#include "objet_visiteur.h"

#include "object_viewer.h"
#include "objet.h"
#include "trou.h"
#include "boule_de_couleur.h"
#include "boule_mortelle.h"
using namespace std;

class Objet;
class Boule;
class Paroi;
class Sol;
class Brique;
class Trou;

void ObjectViewer::
dessiner(Objet& objet)
{
    dispatch::execute(*this, ObjetVisiteur::FamilleObjet(objet));
}



void ObjectViewer::
operator()(BouleDeCouleur& b)
{
    return this->operator()(static_cast<Boule&>(b));
    // slicing pour appeler operator()(Boule&) par défaut
}
void ObjectViewer::
operator()(BouleMortelle& b)
{
    return this->operator()(static_cast<Boule&>(b));
    // slicing pour appeler operator()(Boule&) par défaut
}
