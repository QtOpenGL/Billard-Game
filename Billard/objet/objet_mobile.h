//
//  objet_mobile.h
//  Billard
//
//  Created by Julien on 10/05/2015.
//  Copyright (c) 2015 the billard project. All rights reserved.
//

#ifndef Billard_objet_integrable_h
#define Billard_objet_integrable_h

#include <stdexcept>
#include "objet.h"
#include "integrable.h"
#include "vecteur.h"
#include <iostream>

// ObjetMobile
//
// Cette classe permet d'abstraire les particularités de l'intégrateur
// vis-à-vis des objets physiques que nous manipulons
// et de factoriser le code commun aux objets physiques (position, rotation, etc)
//
class ObjetMobile : public Objet {
private:
    Vecteur position_{0,0,0};
    Vecteur rotation_{0,0,0};
    Vecteur vitesse_ {0,0,0};
    Vecteur vit_angulaire_{0,0,0};
    Vecteur forces_{0,0,0};
    bool objet_en_vie_{true};

public:
    bool en_vie() const { return objet_en_vie_; }
    void set_vie(bool vie){ objet_en_vie_ = vie; }
    
    // Module positionnel
    //
    virtual Vecteur get_position() const { return position_; }
    virtual void    set_position(Vecteur const& p) { position_ = p; }
    
    virtual Vecteur get_vitesse() const { return vitesse_; }
    virtual void    set_vitesse(Vecteur const& v) { vitesse_ = v; }
    
    virtual Vecteur get_acceleration(double temps /*s*/) const = 0;
    
    // Module rotationnel
    //
    virtual Vecteur get_rotation() const { return rotation_; }
    virtual void    set_rotation(Vecteur const& r) { rotation_ = r; }
    
    virtual Vecteur get_vit_angulaire() const { return vit_angulaire_; }
    virtual void    set_vit_angulaire(Vecteur const& va) { vit_angulaire_ = va; }
    
    virtual Vecteur get_acc_angulaire(double temps /*s*/) const = 0; //acceleration
    
    // Attributs physiques
    //
    virtual double  get_masse()  const = 0;
    virtual double  get_volume() const = 0;
    virtual double  get_coef_frot_restitution() const = 0;
    virtual double  get_coef_frot_propre()      const = 0;
    
    virtual Vecteur get_resultante_des_forces() const {return forces_;}
    virtual void    set_resultante_des_forces(Vecteur const& resultante){forces_ = resultante;}

};
#endif
