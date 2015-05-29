//  Fichier : sol.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//

#ifndef __Billard__sol__
#define __Billard__sol__

#include "paroi.h"
class Boule;

class Sol : public Paroi {
public:
    OBJET_VISITABLE
    
    
    Sol(Vecteur origine, Vecteur largeur, Vecteur longueur, double coef_frot_propre = 1, double coef_restitution = 1, double coef_roulement = 0, double coef_glissement = 0);
    
    //methode get :
    double  get_coef_frot_roulement()  const ;
    double  get_coef_frot_glissement() const ;
    
    bool reaction_normale(Boule& b, double seuil_vitesse = SEUIL_VITESSE) const;
    void reaction_de_frottement(Boule& b) const;
    void ajuster_position(Boule& b, double seuil_altitude = SEUIL_DISTANCE) const;
    
private:
    bool sur_le_sol(Boule& b, double seuil_vitesse = SEUIL_VITESSE) const;
    void annuler_vitesse(Boule& b) const;
    void annuler_forces(Boule& b) const;
    double composante_normale(Vecteur const& v) const;
    bool suffisament_proche(Boule& b, double seuil_distance = SEUIL_DISTANCE) const;
    bool suffisament_lent(Boule& b, double seuil_vitesse = SEUIL_VITESSE) const;
    double coef_frot_roulement;
    double coef_frot_glissement;
    bool au_dessus_du_sol(Vecteur const& v) const;
public:
    static constexpr double SEUIL_VITESSE = 1e-6;
    static constexpr double SEUIL_DISTANCE = 1e-3;
    static constexpr double SEUIL_VITESSE_COLLISION = 0.1;
};
// voilà, pas besoin de plus de modifications
// pour les fonctions relatives aux collisions, voir les fichiers : sol.cpp et collision.h

std::ostream& operator << (std::ostream& flux, Sol const& sol);
#endif /* defined(__Billard__sol__) */

