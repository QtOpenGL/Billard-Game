//  Fichier : boule_de_couleur.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//      Cette classe n'est pas implémentée dans le coeur du projet car elle demande
//      la mise en place d'une partie spécifique fortement liée à des détails technologiques (OpenGl, DirectX, Console, etc.)
//
//      Nous implémentons cette classe dans la partie graphique avec Qt et OpenGL


#ifndef Billard_boule_de_couleur_h
#define Billard_boule_de_couleur_h

#include "boule.h"


class Couleur;
class BouleDeCouleur : public Boule
{
public:
    Couleur* couleur;

    BouleDeCouleur(double rayon, double masse, double coef_frot_propre, double coef_restitution,
                       Couleur* couleur);

    ~BouleDeCouleur();
    
    void accept(ObjetVisiteur &v) override;
};


#endif
