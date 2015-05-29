//  Fichier : obstacle_immobile.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__obstacle_immobile__
#define __Billard__obstacle_immobile__

#include "objet.h"
#include "vecteur.h"
#include <numeric>

//
// Classe de base pour les obstacles (brique, paroi, sol, etc...)
//  Cette classe sert à factoriser le code commun à ces objets (position, rotation, etc.)
//  Remarquons de plus que tous les obstacles immobiles doivent avoir une vitesse nulle.
//  Par convention dans ce projet, la masse d'un obstacle est infinie.

class ObstacleImmobile : public Objet{
public:
    ObstacleImmobile(Vecteur position, double volume, double cfrestitution, double cfpropre)
    : position_{position}, forces_{0,0,0}, volume_{volume}, coef_frot_restitution_{cfrestitution}, coef_frot_propre_{cfpropre}, objet_en_vie_{true} {}
    
    //attributs de base d'un objet physique
    virtual Vecteur get_position()              const { return position_; }
    virtual Vecteur get_rotation()              const { return Vecteur::DeTaille(3); }
    virtual double  get_volume()                const { return volume_;   }
    virtual double  get_coef_frot_restitution() const { return coef_frot_restitution_; }
    virtual double  get_coef_frot_propre()      const { return coef_frot_propre_;      }
    
    virtual Vecteur get_vitesse()       const override final { return Vecteur::DeTaille(3); }
    virtual Vecteur get_vit_angulaire() const override final { return Vecteur::DeTaille(3); }
    virtual double  get_masse()         const override final { return std::numeric_limits<double>::infinity(); }
    virtual Vecteur get_acceleration (double) const override final { return Vecteur::DeTaille(3); }
    virtual Vecteur get_acc_angulaire(double) const override final { return Vecteur::DeTaille(3); }
    
    virtual Vecteur get_resultante_des_forces() const { return forces_; }
    virtual void mise_a_jour_resultante_des_forces() {}
    
    virtual void    set_position(Vecteur const&)  {}
    virtual void    set_vitesse (Vecteur const&)  {}
    
    virtual void    set_rotation(Vecteur const&)  {}
    virtual void    set_vit_angulaire(Vecteur const&) {}
    
    virtual void    set_resultante_des_forces(Vecteur const& resultante){ forces_ = resultante; }
    
    virtual bool en_vie() const { return objet_en_vie_; }
    virtual void set_vie(bool vie){ objet_en_vie_ = vie; }
    
    virtual ~ObstacleImmobile(){}

private:
    Vecteur position_;
    Vecteur forces_;
    double  volume_;
    double  coef_frot_restitution_;
    double  coef_frot_propre_;
    bool objet_en_vie_;
};


#endif /* defined(__Billard__obstacle_immobile__) */
