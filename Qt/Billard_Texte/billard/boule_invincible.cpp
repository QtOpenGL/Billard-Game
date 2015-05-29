#include "boule_invincible.h"
#include "boule_de_couleur_couleur.h"

void BouleInvincible::
set_vie(bool vie){
    if(not vie){
        Couleur* touched = new Couleur{QColor(0,0,0), QColor(0,0,0)};
        delete this->couleur;
        couleur = touched;
    }
}

bool BouleInvincible::
en_vie() const
{
    return true;
}

