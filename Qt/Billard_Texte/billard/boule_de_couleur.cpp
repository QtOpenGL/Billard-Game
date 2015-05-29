#include "boule_de_couleur.h"
#include "boule_de_couleur_couleur.h"
#include "boule.h"
#include <QColor>


BouleDeCouleur::
BouleDeCouleur(double rayon, double masse, double coef_frot_propre, double coef_restitution, Couleur *couleur_owned)
: Boule(rayon, masse, coef_frot_propre, coef_restitution)
, couleur{couleur_owned}
{
    if(couleur == nullptr){
        throw std::runtime_error{"Impossible d'utiliser une couleur nullptr"};
    }
}

BouleDeCouleur::
~BouleDeCouleur()
{
    delete couleur;
}

void BouleDeCouleur::accept(ObjetVisiteur &v)
{
    v.visit(*this);
}

