//  Fichier : dessinable.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__dessinable__
#define __Billard__dessinable__

#include "multiple_dispatch.h"

class Objet;
class Boule;
class Paroi;
class Sol;
class Brique;
class BouleDeCouleur;
class BouleMortelle;
class Trou;

class ObjetVisiteur{
public:
    using FamilleObjet = dispatch::Family<ObjetVisiteur, Objet, Boule, Paroi, Sol, Brique, Trou,
                        BouleDeCouleur, BouleMortelle>;
    
    virtual void visit(Boule&)  = 0;
    virtual void visit(Paroi&)  = 0;
    virtual void visit(Sol&)    = 0;
    virtual void visit(Brique&) = 0;
    virtual void visit(Trou&)   = 0;
    virtual void visit(BouleDeCouleur&) = 0;
    virtual void visit(BouleMortelle&) = 0;
    
    virtual ~ObjetVisiteur(){}
};

#define OBJET_VISITABLE \
virtual void accept(ObjetVisiteur& v) override \
{ v.visit(*this); }
//------------------------------------------------------



#endif /* defined(__Billard__dessinable__) */
