//  Fichier : integrable.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//      Un integrable est un objet qui peut-etre integré par un integrateur
//

#ifndef Billard_integrable_h
#define Billard_integrable_h

#include "vecteur.h"

class Integrable{
public:
    virtual Vecteur etat() const = 0;
    virtual Vecteur derivee_etat() const = 0;
    virtual Vecteur derivee_seconde_etat(double temps) const = 0;
    
    virtual void set_etat(Vecteur const& nouveau) = 0;
    virtual void set_derivee_etat(Vecteur const& nouveau) = 0;
    
    virtual ~Integrable(){}
};
#endif
