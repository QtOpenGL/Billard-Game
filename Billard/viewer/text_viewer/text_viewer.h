//  Fichier : text_viewer.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__support_a_dessin__text__
#define __Billard__support_a_dessin__text__

#include "object_viewer.h"

class TextViewer : public ObjectViewer{
public:
    //void operator()(Objet &);
    void operator()(Boule &) override ;
    void operator()(Paroi&) override ;
    void operator()(Sol   &) override ;
    void operator()(Brique&) override;
    void operator()(Trou&) override;
};



#endif /* defined(__Billard__support_a_dessin__) */
