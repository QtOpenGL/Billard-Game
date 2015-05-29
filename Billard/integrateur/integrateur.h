//  Fichier : integrateur.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__integrateur__
#define __Billard__integrateur__

#include "integrable.h"

class Integrateur{
public:
    virtual void integre(Integrable& objet, double temps_depart, double dt_recherche_max)
        const = 0 ;
    virtual ~Integrateur(){}
};

class Integrateur_Euler : public Integrateur {
public:
    virtual void integre(Integrable& objet, double temps, double dt) const override;
};

class Integrateur_Newmark : public Integrateur {
public:
    virtual void integre(Integrable& objet, double temps, double dt) const override;
};

class Integrateur_Newmark_Simple : public Integrateur {
public:
    virtual void integre(Integrable& objet, double temps, double dt) const override;
};;

class Integrateur_Runge_Kutta4 : public Integrateur {
public:
    virtual void integre(Integrable& objet, double temps, double dt) const override;
};

#endif /* defined(__Billard__integrateur__) */