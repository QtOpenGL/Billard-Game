//  Fichier : objet.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#ifndef __Billard__objet__
#define __Billard__objet__

#include "vecteur.h"
#include "objet_visiteur.h"
#include "integrable.h"

class Integrateur;
class Objet;
class Boule;

//
// A\
// La classe Objet représente un objet physique
// Comme tout objet physique dans la vraie vie,
// elle possède (1) une masse, (2) un volume, (3) une position, (4) une vitesse, etc.
//
// B\
// Dans la vraie vie, les objets immobiles, ne sont pas différents des objets mobiles.
// Ils ont simplement une vitesse nulle.
// C'est ce que nous ferons dans ce projet.
//
// C\
// La class Objet est une interface dans laquelle
// les détails d'implémentation ne nous interesse pas
// Nous utilisons donc des méthodes virtuelles pures,
// conformément au point de vue exposé dans mon article sur la POO.
//
// D\
// Le motif de conception visiteur est bien connu en programmation.
// Voir les innombrables articles dédiés à ce sujet
// Voir la bibliothèque Multiple Dispatch Wrapper à l'adresse :
// https://bitbucket.org/Gauss_/library-multiple-dispatch-in-cpp
// En particulier, cette bibliothèque contient des fichiers d'exemples
// dont l'étude pourra se révéler utile.
//
// E\ Remarques
// On pourra eventuellement rajouter d'autres attributs physiques si besoin
// Moment cinétique, torsion, etc...
//

class UniqId{
public:
    const unsigned uniq_id = UniqId::next_id();
    UniqId& operator=(UniqId const&){ return *this; }

private:
    static unsigned next_id(){ static unsigned cur_id = 0; return cur_id++; }
};

struct PointEtPlan{
    Vecteur point;
    Vecteur normal_au_plan_tangent;
};

class Objet : public Integrable, public UniqId{
public:
    virtual void accept(ObjetVisiteur& v) = 0;
    virtual PointEtPlan point_le_plus_proche_de(Vecteur const& point) const = 0;
    
    virtual bool en_vie() const    = 0;
    virtual void set_vie(bool vie) = 0;
    
    //attributs de base d'un objet physique
    virtual double  get_coef_frot_restitution() const = 0;
    virtual double  get_coef_frot_propre()      const = 0;
    virtual double  get_masse()    const = 0;
    virtual double  get_volume()   const = 0;
    
    virtual Vecteur get_position()     const = 0;
    virtual Vecteur get_vitesse()      const = 0;
    virtual Vecteur get_acceleration(double temps) const = 0;
    
    virtual Vecteur get_rotation()      const = 0;
    virtual Vecteur get_vit_angulaire() const = 0;
    virtual Vecteur get_acc_angulaire(double temps)  const = 0;
    
    virtual Vecteur get_resultante_des_forces() const = 0;
    virtual void mise_a_jour_resultante_des_forces() = 0;
    
    virtual void    set_position(Vecteur const& p)  = 0;
    virtual void    set_vitesse (Vecteur const& v)  = 0;
    
    virtual void    set_rotation(Vecteur const& r)  = 0;
    virtual void    set_vit_angulaire(Vecteur const& va) = 0;
    
    virtual void    set_resultante_des_forces(Vecteur const& resultante) = 0;
    
    double energie_cinetique() const { return 0.5 * get_masse() * norme_carree( get_vitesse() ); }
    
    // Méthodes de compatibilité avec l'interface Integrable
    //
    static constexpr unsigned DIM_ETAT = 6;
    
    virtual Vecteur etat()         const override;
    virtual Vecteur derivee_etat() const override;
    virtual Vecteur derivee_seconde_etat(double temps) const override;
    
    virtual void set_etat(Vecteur const& nouveau) override;
    virtual void set_derivee_etat(Vecteur const& nouveau) override;
    
    //destructeur
    virtual ~Objet(){}

};

inline Vecteur Objet::
etat() const
{
    return concatenation(get_position(), get_rotation());
}
inline Vecteur Objet::
derivee_etat() const
{
    return concatenation(get_vitesse(), get_vit_angulaire());
}
inline Vecteur Objet::
derivee_seconde_etat(double temps) const
{
    return concatenation(get_acceleration(temps), get_acc_angulaire(temps));
}
inline void Objet::
set_etat(Vecteur const& nouveau)
{
    if(nouveau.size() != DIM_ETAT){ throw std::length_error{"mauvaise dimension"}; }
    set_position(Vecteur{nouveau[0], nouveau[1], nouveau[2]});
    set_rotation(Vecteur{nouveau[3], nouveau[4], nouveau[5]});
}
inline void Objet::
set_derivee_etat(Vecteur const& nouveau)
{
    if(nouveau.size() != DIM_ETAT){ throw std::length_error{"mauvaise dimension"}; }
    set_vitesse(Vecteur{nouveau[0], nouveau[1], nouveau[2]});
    set_vit_angulaire(Vecteur{nouveau[3], nouveau[4], nouveau[5]});
}

#endif /* defined(__Billard__objet__) */
