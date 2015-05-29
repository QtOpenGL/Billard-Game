//  Fichier : viewer.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__viewer__
#define __Billard__viewer__

class Objet;

class Viewer {
public:
    virtual void dessiner(Objet &) = 0;
};

class DummyViewer: public Viewer {
public:
    virtual void dessiner(Objet &) {}
};

#endif /* defined(__Billard__viewer__) */
