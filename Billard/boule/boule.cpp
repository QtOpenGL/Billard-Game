//  Fichier : boule.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Implémentation de la classe Boule demandée
//
#include <iostream>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <limits>
#include <tuple>

#include "boule.h"
#include "vecteur.h"
#include "string_matcher.h"
#include "raii.h"
#include "objet.h"
#include "almost_equal.h"
using namespace std;

// ==========================
// some helper functions for error handling

void Boule::
verifier_dimension(const Vecteur& nouveau_copy)
{
    if(nouveau_copy.size() != Boule::DIM_ETAT)
    {
        throw std::length_error{"Le vecteur d'état doit etre de dimension 6"};
    }
}


// ==========================
// here is the implementation

Boule::
Boule (double rayon, double masse, double coef_frot_propre, double coef_restitution)
: normal_au_support    { 0,0,0                  }
, rayon_               { rayon                  }
, masse_               { masse                  }
, coef_restitution_    { coef_restitution       }
, coef_frot_propre_    { coef_frot_propre       }
, coef_frot_roulement_ { 0                      }
, coef_frot_glissement_{ 0                      }
, calculateur_de_poids { [](double m){ return Vecteur{0, 0, -m*9.81}; } }
{
    if(rayon == 0) { throw RayonNulException{}  ; }
    if(masse == 0) { throw MasseNulleException{}; }
    //set_resultante_des_forces(calcul_force_ext(masse));
    //
    // Nous avons décidé de ne pas initialiser la resultante des forces avec le poids
    // Car cette responsabilité n'appartient pas a la boule, mais au monde physique
    // par exemple, sur mars, la constante de gravité n'est pas le meme
    //
    // Sur le plan pratique, le poids est ajouté par la methode evolue() du billard
    // en meme temps que la reaction du sol
}

Boule Boule::
Nouvelle(double rayon, double masse, double coef_frot_propre, double coef_restitution,
                      Vecteur const& etat, Vecteur const& derive_etat)
{
    Boule nouvelle_boule{rayon, masse, coef_frot_propre, coef_restitution};
    nouvelle_boule.set_etat(etat);
    nouvelle_boule.set_derivee_etat(derive_etat);
    return nouvelle_boule;
}

double
lire_double_depuis(std::istream& flux, std::string end_of_line = "")
{
    using namespace string_matcher;
    
    double valeur;
    flux >> std::noskipws >> valeur >> StringPattern{end_of_line};
    return valeur;
}

Vecteur
lire_vecteur_depuis(size_t taille, std::istream& flux, std::string end_of_line = "")
{
    using namespace string_matcher;
    
    Vecteur retour = Vecteur::DeTaille(taille);
    
    if(taille == 0) return retour;
    
    for(int i = 0; i < taille - 1; ++i){
        flux >> std::noskipws >> retour[i] >> StringPattern{", "};
    }
    flux >> std::noskipws >> retour[ taille - 1 ] >> StringPattern{end_of_line};
    return retour;
}

Boule Boule::
LireDepuis (std::istream& flux_a_lire)
{
    // Gestion des erreurs de lecture
    auto exception_policy = flux_a_lire.exceptions();
    auto finally = RAII( [&](){ flux_a_lire.exceptions( exception_policy );} ); //equivalent du finally en Java
    flux_a_lire.exceptions ( istream::failbit | istream::badbit | istream::eofbit );
    
    // Lecture
    double  masse       = lire_double_depuis (flux_a_lire,   " # masse\n");
    double  rayon       = lire_double_depuis (flux_a_lire,   " # rayon\n");
    Vecteur position    = lire_vecteur_depuis(3, flux_a_lire," # position\n");
    Vecteur angle       = lire_vecteur_depuis(3, flux_a_lire," # angle\n");
    Vecteur vitesse     = lire_vecteur_depuis(3, flux_a_lire," # vitesse\n");
    Vecteur vit_ang     = lire_vecteur_depuis(3, flux_a_lire," # vitesse angulaire\n");
    Vecteur f_ext       = lire_vecteur_depuis(3, flux_a_lire," # force ext\n");
    double  restitution = lire_double_depuis (flux_a_lire,   " # restitution\n");
    double  frt_prp     = lire_double_depuis (flux_a_lire,   " # coef frot propre\n");
    double  frt_rou     = lire_double_depuis (flux_a_lire,   " # frot roulement\n");
    double  frt_gli     = lire_double_depuis (flux_a_lire,   " # frot glissement\n");
    
    // Création de la boule
    Boule  boule_lue{rayon, masse, frt_prp, frt_gli};
    
    if(boule_lue.get_masse() != masse){
        throw ValeursNumeriquesIncoherentes{};
    }
    
    boule_lue.set_etat        ( concatenation( position , angle   ));
    boule_lue.set_derivee_etat( concatenation( vitesse  , vit_ang ));
    boule_lue.set_resultante_des_forces( f_ext );
    boule_lue.coef_restitution_     = restitution;
    boule_lue.coef_frot_propre_     = frt_prp;
    boule_lue.coef_frot_roulement_  = frt_rou;
    boule_lue.coef_frot_glissement_ = frt_gli;
    
    return boule_lue;
}

double Boule::
get_coef_frot_roulement () const
{
    return coef_frot_roulement_;
}
double Boule::
get_coef_frot_glissement () const
{
    return coef_frot_glissement_;
}

double  Boule::
get_coef_frot_restitution ()const
{
    return coef_restitution_; 
}

double Boule::
get_coef_frot_propre()const
{
    return coef_frot_propre_; 
}

double Boule::
get_volume ()const
{
    double volume = (4*rayon_*rayon_*rayon_*M_PI)/3;
    return volume;
}

double Boule::
get_masse() const
{
    return masse_;
}

double Boule::
get_rayon() const
{
    return rayon_; 
}

void Boule::
set_coef_frot_roulement (double nouveau)
{
    coef_frot_roulement_ = nouveau;
}

void Boule::
set_coef_frot_glissement (double nouveau)
{
    coef_frot_glissement_ = nouveau;
}

void Boule::
set_coef_frot_restitution (double nouveau)
{
    coef_restitution_ = nouveau;
}
void Boule::
set_coef_frot_propre      (double nouveau)
{
    coef_frot_propre_ = nouveau;
}
void Boule::
set_support(Vecteur const& normal)
{
    normal_au_support = normal;
}
void Boule::
ajouter_force(Vecteur const& df)
{
    Vecteur resultante = get_resultante_des_forces();
    set_resultante_des_forces(resultante + df);
}

Vecteur Boule::
get_acceleration(double t/*s*/) const
{   // omega'' = f(t, omega, omega')
    (void) t;
    Vecteur position_seconde = get_resultante_des_forces() / masse_;
    return position_seconde;
}

Vecteur Boule::
get_acc_angulaire(double t/*s*/) const
{
    (void) t;
    Vecteur angle_seconde = moment_des_forces_exterieures() / moment_inertie();
    return angle_seconde;
}

void Boule::
mise_a_jour_resultante_des_forces()
{
    Vecteur glissement = calculer_frottement_de_glissement();
    Vecteur roulement  = calculer_frottement_de_roulement();
    
    set_resultante_des_forces(calculateur_de_poids(get_masse()) + glissement + roulement);
}

double  Boule::
moment_inertie() const
{
    return (2./5. * masse_ * rayon_ * rayon_) ;
}

Vecteur Boule::
moment_des_forces_exterieures() const
{
    // hypotheses :
    // (1) il serait absurde de calculer le moment par rapport au point O
    // (2) par rapport au centre de masse G, le moment du poids est nul
    return calculer_moment_frottement() + calculer_moment_glissement();
}

template <class CompVect, class CompDouble>
bool Boule::
compare_with(Boule const& rhs, CompVect comp_vect, CompDouble comp_double) const
{
    return
    comp_vect( get_position() , rhs.get_position())
    && comp_vect( get_vitesse()  , rhs.get_vitesse())
    && comp_vect( get_rotation() , rhs.get_rotation())
    && comp_vect( get_vit_angulaire()    , rhs.get_vit_angulaire())
    && comp_vect( get_resultante_des_forces() , rhs.get_resultante_des_forces())
    
    && comp_double(rayon_        , rhs.rayon_)
    && comp_double(masse_        , rhs.masse_)
    && comp_double(coef_restitution_     , rhs.coef_restitution_)
    && comp_double( coef_frot_propre_    , rhs.coef_frot_propre_)
    && comp_double( coef_frot_roulement_ , rhs.coef_frot_roulement_)
    && comp_double( coef_frot_glissement_, rhs.coef_frot_glissement_);
}

Vecteur Boule::
vitesse_au_point_de_contact(Vecteur const& normal) const
{
    return get_vitesse() + rayon_ * produit_vectoriel( normal ,get_vit_angulaire());
}

bool Boule::
roulement_sans_glissement() const
{
    double norme = norme_carree( vitesse_au_point_de_contact(normal_au_support) );
    return norme <= 1e-6;
}

Vecteur Boule::
calculer_frottement_de_glissement() const
{
    Vecteur glissement {0,0,0};
    
    if(not roulement_sans_glissement())
    {
        Vecteur vitesse_unitaire = vitesse_au_point_de_contact(normal_au_support);
        vitesse_unitaire /= norme(vitesse_unitaire);
        
        glissement = - masse_ * 9.81 * coef_frot_glissement_ * vitesse_unitaire;
    }
    return  glissement;
}

Vecteur Boule::
calculer_moment_glissement()const
{
    return rayon_ * produit_vectoriel( calculer_frottement_de_glissement(), normal_au_support);
}


Vecteur Boule::
calculer_frottement_de_roulement()const
{
    Vecteur frottement_de_roulement {0,0,0};
    
    if(roulement_sans_glissement())
    {
        Vecteur vitesse_u = get_vitesse();
        double norme_vitesse = norme(vitesse_u);
        if(not almost_equal(norme_vitesse, 0.0, 10)){
            vitesse_u /= norme_vitesse;
        }
        frottement_de_roulement = -masse_ * 9.81 * coef_frot_roulement_* vitesse_u;
    }
    return frottement_de_roulement;
}

Vecteur Boule::
calculer_moment_frottement() const
{
    Vecteur vitesse_de_rotation_u = get_vit_angulaire();
    double norme_r = norme(vitesse_de_rotation_u);
    
    if(not almost_equal(norme_r, 0.0, 10))
    {
        vitesse_de_rotation_u /= norme_r;
        double mu_r = coef_frot_roulement_ * rayon_;
        return -masse_*9.81* mu_r * vitesse_de_rotation_u;
    }else{
        return Vecteur{0,0,0};
    }
#warning "Vérifier qu'il s'agit bien du coef de frottement"
}
PointEtPlan Boule::
point_le_plus_proche_de(Vecteur const& point_exterieur) const
{
    Vecteur ecart_centres_unitaire = unitaire(point_exterieur - get_position());
    return {get_position() + get_rayon()*ecart_centres_unitaire, ecart_centres_unitaire};
}

bool Boule::
operator==(Boule const& rhs) const
{
    return compare_with(rhs
                        , [&](Vecteur const& v1, Vecteur const& v2){ return v1 == v2;}
                        , [&](double d1, double d2){ return d1 == d2; }
                        );
}
bool Boule::
epsilon(Boule const& rhs, unsigned int precision) const
{
    return compare_with(rhs
                        , [&](Vecteur const& v1, Vecteur const& v2){ return ::epsilon(v1, v2, precision);}
                        , [&](double d1, double d2){ return ::almost_equal(d1, d2, precision); }
                        );
}


std::ostream& Boule::
vers_le_flux(std::ostream& flux) const
{
    const auto current_precision = flux.precision();
    auto p = std::setprecision(numeric_limits<double>::digits10 + 1);
    auto finally = RAII(  [&](){ flux << std::setprecision(static_cast<int>(current_precision)); }  );
    //besoin d'un cast... la logique de la std m'épate...
    
    flux << p << masse_                          << " # masse"            << endl;
    flux << p << rayon_                          << " # rayon"            << endl;
    flux << p << get_position()                  << " # position"         << endl;
    flux << p << get_rotation()                  << " # angle"            << endl;
    flux << p << get_vitesse()                   << " # vitesse"          << endl;
    flux << p << get_vit_angulaire()             << " # vitesse angulaire"<< endl;
    flux << p << get_resultante_des_forces()     << " # force ext"        << endl;
    flux << p << coef_restitution_               << " # restitution"      << endl;
    flux << p << coef_frot_propre_               << " # coef frot propre" << endl;
    flux << p << coef_frot_roulement_            << " # frot roulement"   << endl;
    flux << p << coef_frot_glissement_           << " # frot glissement"  << endl;
    
    return flux;
}

std::ostream&
operator << (std::ostream& flux, Boule const& boule)
{
    return boule.vers_le_flux(flux);
}
void Boule::
changer_champs_attraction(std::function<Vecteur(double)> champs_de_force)
{
    std::swap(calculateur_de_poids , champs_de_force);
}

