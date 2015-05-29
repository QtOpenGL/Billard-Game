//  Fichier : boule.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Déclaration de la classe Boule demandée
//
#ifndef PROJET_EPFL_BOULE_H
#define PROJET_EPFL_BOULE_H

#include <iostream>
#include <stdexcept>
#include <functional>

#include "vecteur.h"
#include "objet_mobile.h"

//
// Une Boule est une spécialisation de la classe objet
// Les informations spécifiques à la Boule sont ajoutées
// Par exemple, une boule est ronde (rayon), etc.
//
class Boule : public ObjetMobile{
public:
    
    OBJET_VISITABLE
    
    // Constructeurs pour la classe
    //
    Boule (double rayon, double masse, double coef_frot_propre, double coef_restitution);
    
    static Boule
    Nouvelle(double rayon, double masse, double coef_frot_propre, double coef_restitution,
             Vecteur const& etat = Vecteur::DeTaille(DIM_ETAT), Vecteur const& derive_etat = Vecteur::DeTaille(DIM_ETAT));
    
    static Boule
    LireDepuis (std::istream& flux_a_lire);
    
    Boule (Boule const&) = default;
    Boule (Boule&&) = default;
    Boule& operator=(Boule const&) = default;
    Boule& operator=(Boule&&) = default;
    ~Boule() = default;
    

    // Equations du mouvement
    //
    
    virtual Vecteur get_acceleration(double temps /*s*/) const override;
    virtual Vecteur get_acc_angulaire(double temps /*s*/) const override; //acceleration
    void mise_a_jour_resultante_des_forces() override;
    
    // Attributs physiques
    //
    
    double get_rayon() const;
    virtual double  get_masse()  const override;
    virtual double  get_volume() const override;
    
    double  get_coef_frot_roulement()  const ;
    double  get_coef_frot_glissement() const ;
    double  get_coef_frot_restitution()const ;
    double  get_coef_frot_propre()     const ;
    void    set_coef_frot_roulement   (double  nouveau);
    void    set_coef_frot_glissement  (double  nouveau);
    void    set_coef_frot_restitution (double  nouveau);
    void    set_coef_frot_propre      (double  nouveau);
    
    // Autres méthodes
    //
    PointEtPlan point_le_plus_proche_de(Vecteur const& point_exterieur) const override;
    void set_support(Vecteur const& normal_au_support);
    void ajouter_force(Vecteur const& nouv_force);
    std::ostream& vers_le_flux (std::ostream& flux) const;
    Vecteur vitesse_au_point_de_contact      (const Vecteur& normal_au_point) const;
    
    void changer_champs_attraction(std::function<Vecteur(double)> champs_de_force);
    // Méthodes de comparaisons
    //
    
    bool operator== (Boule const& autre_boule) const;
    bool epsilon    (Boule const& autre_boule, unsigned int precision) const; //comparaison a epsilon pres
    template <class CompVect, class CompDouble>
    bool compare_with(Boule const& autre, CompVect comp_vect, CompDouble comp_double) const;
    
private:
    double  moment_inertie()                const;
    Vecteur moment_des_forces_exterieures() const;
    static  void verifier_dimension(const Vecteur& nouveau_copy);
    
    Vecteur calculer_frottement_de_glissement() const;
    Vecteur calculer_moment_glissement       () const;
    Vecteur calculer_frottement_de_roulement () const;
    Vecteur calculer_moment_frottement       () const;
    bool roulement_sans_glissement           () const;
    
private:
    //attributs de la classe
    Vecteur normal_au_support;
    
    const double  rayon_ ;
    const double  masse_;
    
    double coef_restitution_ ;
    double coef_frot_propre_ ;
    double coef_frot_roulement_ ;
    double coef_frot_glissement_ ;
    std::function<Vecteur(double)> calculateur_de_poids;
};

class BouleEspion : public Boule{
public:
    using Boule::Boule;
    virtual void    set_position(Vecteur const& p) override
    {
        std::cout << "| set_pos(" << p << ")\n";
        Boule::set_position(p);
    }
    virtual void    set_vitesse (Vecteur const& v) override
    {
        std::cout << "| set_vit(" << v << ")\n";
        Boule::set_vitesse(v);
    }
    virtual void    set_rotation(Vecteur const& r) override
    {
        std::cout << "| set_rotation(" << r << ")\n";
        Boule::set_rotation(r);
    }
    virtual void    set_vit_angulaire(Vecteur const& va) override
    {
        std::cout << "| set_vit_ang(" << va << ")\n";
        Boule::set_vit_angulaire(va);
    }
    virtual void    set_resultante_des_forces(Vecteur const& resultante) override
    {
        std::cout << "| set_resultante(" << resultante << ")" << std::endl;
        Boule::set_resultante_des_forces(resultante);
    }
    virtual void set_vie(bool vie) override
    {
        std::cout << "| en vie : " << vie << std::endl;
        Boule::set_vie(vie);
    }
    
};
//
// Factory
//
namespace BouleFactory {
    inline Boule boule_de_billard_americain(Vecteur const& etat = Vecteur::DeTaille(Boule::DIM_ETAT), Vecteur const& detat = Vecteur::DeTaille(Boule::DIM_ETAT))
    {
        return Boule::Nouvelle(5.7/*cm*/, 162/*g*/, 1, 1, etat, detat);
    }
    inline Boule boule_de_billard_francais(Vecteur const& etat = Vecteur::DeTaille(Boule::DIM_ETAT), Vecteur const& detat = Vecteur::DeTaille(Boule::DIM_ETAT))
    {
        return Boule::Nouvelle(6.1/*cm*/, 209/*g*/, 1, 1, etat, detat);
    }
    inline Boule boule(double rayon, Vecteur etat = Vecteur::DeTaille(Boule::DIM_ETAT), Vecteur detat = Vecteur::DeTaille(Boule::DIM_ETAT))
    {
        return Boule::Nouvelle(rayon, 1, 1/*100%*/, 1/*100%*/, etat, detat);
    }
}
//
// Exceptions
//
struct MasseNulleException : public std::logic_error
{
    MasseNulleException()  : std::logic_error{"Impossible de créer une boule de masse nulle"} {}
};
struct RayonNulException : public std::logic_error
{
    RayonNulException()  : std::logic_error{"Impossible de créer une boule de rayon nul"} {}
};
struct ValeursNumeriquesIncoherentes : public std::runtime_error
{
    ValeursNumeriquesIncoherentes()  : std::runtime_error{"Impossible de créer une boule : Valeurs numériques incohérentes"} {}
};

//
// Fonctions et opérateurs
//
std::ostream& operator << (std::ostream& flux, Boule const& boule);




#endif //PROJET_EPFL_BOULE_H