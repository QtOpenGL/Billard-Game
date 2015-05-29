//  Fichier : plan.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//

#include <iostream>
#include <cmath>
#include <iomanip>
#include "sol.h"
#include "boule.h"
#include "collision.h"
#include "almost_equal.h"
using namespace std;

Sol::
Sol(Vecteur origine, Vecteur largeur, Vecteur longueur, double coef_frot_propre, double coef_restitution, double coef_roulement, double coef_glissement)
: Paroi(origine, largeur, longueur ,coef_restitution, coef_frot_propre)
, coef_frot_glissement(coef_glissement)
, coef_frot_roulement(coef_roulement)
{}

double  Sol::
get_coef_frot_roulement()  const
{
    return coef_frot_roulement;
}
double Sol:: get_coef_frot_glissement() const
{
    return coef_frot_glissement;
}


double Sol::
composante_normale(Vecteur const& v) const
{
    return produit_scalaire(v, normal_unitaire_a_la_paroi() );
}
bool Sol::
suffisament_proche(Boule& b, double seuil_distance) const
{
    //Vecteur vecart = CalculateurDeEcart{}.entre(b, *this);
    //double ecart = norme(vecart);
    //return ecart <= seuil_distance;
    Vecteur position = b.get_position();
    Vecteur OP = position - this->get_position();
    double altitude = composante_normale(OP) - b.get_rayon();
    return altitude <= seuil_distance;
}
bool Sol::
suffisament_lent(Boule& b, double seuil_vitesse) const
{
    double vitesse_normale = composante_normale(b.get_vitesse());
    return abs(vitesse_normale) <= seuil_vitesse;
}

bool Sol::
sur_le_sol(Boule& b, double seuil_vitesse) const
{
    return suffisament_lent(b, seuil_vitesse) and suffisament_proche(b);
}

bool Sol::
au_dessus_du_sol(Vecteur const& exterieur) const
{
    // Cette méthode ne marche pas si les axes ne sont pas orthogonaux
    Vecteur v = exterieur - get_position();
    double x = produit_scalaire(v, e1);
    double y = produit_scalaire(v, e2);
    return 0 <= x and x <= largeur
       and 0 <= y and y <= longueur;
}
void Sol::
ajuster_position(Boule& b, double seuil_altitude) const
{
    if( not au_dessus_du_sol(b.get_position())){
        return;
    }
    Vecteur normal = this->normal_unitaire_a_la_paroi();

    // On plaque la boule contre le sol
    Vecteur position = b.get_position();

    Vecteur OP = position - this->get_position();
    double altitude = composante_normale(OP);
    if(altitude <= seuil_altitude + b.get_rayon()){
        double altitude_voulue = b.get_rayon();
        Vecteur nouveau_OP = OP + (altitude_voulue - altitude) * normal;
        Vecteur nouvelle_position = get_position() + nouveau_OP;
        b.set_position(nouvelle_position);
    }
}
void Sol::
annuler_vitesse(Boule& b) const
{
    Vecteur normal = this->normal_unitaire_a_la_paroi();
    
    // On annule complètement sa vitesse normale
    Vecteur vitesse = b.get_vitesse();
    double vitesse_normale = composante_normale(vitesse);
    Vecteur nouvelle_vitesse = vitesse - vitesse_normale * normal;
    b.set_vitesse(nouvelle_vitesse);
}
void Sol::
annuler_forces(Boule& b) const
{
    Vecteur normal = this->normal_unitaire_a_la_paroi();
    
    // On calcule la réaction normale
    double resultante_normale = composante_normale( b.get_resultante_des_forces() );
    Vecteur reaction = - resultante_normale * normal;
    b.ajouter_force(reaction);
}

bool Sol::
reaction_normale(Boule& b, double seuil_vitesse) const
{
    ajuster_position(b);
    
    if( sur_le_sol(b, seuil_vitesse) )
    {
        annuler_forces  (b);
        annuler_vitesse (b);
        return true;
    }
    return false;
}

void Sol::
reaction_de_frottement(Boule& b) const
{
    if( suffisament_proche(b) )
    {
        // le sol affectera les coefficients frt_roul et frt_glis de la boule
        // en multipliant ses propres coefficients avec le coeff_frt de la boule
        
        b.set_coef_frot_roulement ( coef_frot_roulement *  b.get_coef_frot_propre()  );
        b.set_coef_frot_glissement( coef_frot_glissement * b.get_coef_frot_propre() );
        b.set_support( this->normal_unitaire_a_la_paroi() );
    }else
    {
        b.set_coef_frot_roulement (0.);
        b.set_coef_frot_glissement(0.);
        b.set_support(Vecteur{0,0,0});
    }
}

std::ostream& operator << (std::ostream& flux, Sol const& sol)
{
    flux << "un sol    : "                                                      << " # " << std::endl;
    
    flux << "d'origine : "                   << sol.get_position()              << " # " << std::endl;
    flux << "de longeure : "                 << sol.get_longueur ()             << " # " << std::endl;
    flux << "de largueur : "                 << sol.get_largeur()               << " # " << std::endl;
    flux << "avec un coef de restitution : " << sol.get_coef_frot_restitution() << " # " << std::endl;
    flux << "avec un coef prpore : "         << sol.get_coef_frot_propre()      << " # " << std::endl;
    
    flux << "coef_fr_glissement : "          << sol.get_coef_frot_glissement()  << " # " << std::endl;
    flux << "coef_fr_roulement :  "          << sol.get_coef_frot_roulement()   << " # " << std::endl;
    
    return flux;
}
