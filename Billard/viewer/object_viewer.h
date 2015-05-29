//  Fichier : object_viewer.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__object_viewer__
#define __Billard__object_viewer__

#include "viewer.h"

class Objet;
class Boule;
class Paroi;
class Sol;
class Brique;
class BouleDeCouleur;
class BouleMortelle;
class Trou;
class ObjectViewer : public Viewer{
public:
    void dessiner(Objet&) override final;
    
    virtual void operator()(Boule &) = 0;
    virtual void operator()(Paroi&) = 0;
    virtual void operator()(Sol   &) = 0;
    virtual void operator()(Brique&) = 0;
    virtual void operator()(Trou&) = 0;
    virtual void operator()(BouleDeCouleur&);
    virtual void operator()(BouleMortelle&);
};


#endif /* defined(__Billard__object_viewer__) */
