#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <vector>

#include "gestion_collision.h"
#include "trinome_second_degre.h"
#include "almost_equal.h"

#include "vecteur.h"
#include "boule.h"
#include "objet.h"
#include "obstacle_immobile.h"
#include "integrable.h"
#include "integrateur.h"
#include "detection_collision.h"

using namespace std ;
bool DEBUGbool = false;

//****************************************************************************//
// Fonction collision Boule/Boule
//****************************************************************************//

Vecteur
calcul_delta_v(bool test, double m1, double m2, Vecteur const& v_c, Vecteur const& n, double lambda, double mu, double v)
{
    if(test)
    {
        return (lambda * v * n)-(2 * m2/(7*(m1 + m2)))*v_c;
    }
    else // Vc forcement non nul
    {
        if(DEBUGbool and norme(v_c) == 0){return (lambda * v * n)-(2 * m2/(7*(m1 + m2)))*v_c;}
        return lambda * v * ( n - (mu * v_c/norme(v_c)));
    }
}

Vecteur
calcul_delta_w(bool test, double m1, double m2, Vecteur const& v_c, Vecteur const& n, double lambda, double mu,
               double v, double r1)
{
    if(test)
    {
        return (5.*m2 / (7. *r1*(m1 + m2)))* produit_vectoriel(n, v_c);
    }else // Vc forcement non nul
    {
        if(DEBUGbool and norme(v_c) == 0){return (5.*m2 / (7. *r1*(m1 + m2)))* produit_vectoriel(n, v_c);}
        return ((5. / 2.*r1)* mu * lambda * v) * produit_vectoriel(n, v_c/norme(v_c));
    }
}
Vecteur
calcul_v_relative_point_choc(const Boule& b1, const Boule& b2, Vecteur const& normal, double vit_normale)
{   // Vc = v1 - v2 + v*n + n ^ (r1w1 + r2w2)
    const Vecteur v1 = b1.get_vitesse();
    const Vecteur v2 = b2.get_vitesse();
    const Vecteur w1 = b1.get_vit_angulaire();
    const Vecteur w2 = b2.get_vit_angulaire();
    const double  r1 = b1.get_rayon();
    const double  r2 = b2.get_rayon();
    
    return v1-v2 + vit_normale * normal + produit_vectoriel(normal, r1*w1 + r2*w2);
}


double
calcul_lambda(double alpha, double m1, double m2) // lambda = (1+alpha)*m2/(m1+m2)
{
    //la masse d'un objet n'est jamais nulle.
    return (1.+alpha)* (m2 / (m1 + m2));
}

bool
test (double alpha, double mu, double vit_normale, Vecteur const& v_c)
{
    return (7. * mu * (1.+alpha) * vit_normale) >= (2. * norme(v_c));
}

std::pair<Vecteur, Vecteur>
calcul_deltaV_deltaW(Boule const& b1, Boule const& b2)
{
    const Vecteur ecart = b1.get_position() - b2.get_position();
    const double norme_ecart = norme(ecart);
    
    if (norme_ecart == 0) {
       return {0. *b2.get_vitesse(), 0. *b2.get_vit_angulaire()};
    // On utilise les methodes get pour avoir un vecteur de la bonne dimension
    }
    const Vecteur normal = ecart / norme_ecart; //norme non nulle, test au dessus.
    
    const double m1 = b1.get_masse();
    const double m2 = b2.get_masse();
    const double r1 = b1.get_rayon();
    
    const double  vitesse_normale = produit_scalaire(normal, b2.get_vitesse() - b1.get_vitesse() );
    const Vecteur v_c = calcul_v_relative_point_choc(b1, b2, normal, vitesse_normale);
    
    const double alpha  = b1.get_coef_frot_restitution() * b2.get_coef_frot_restitution();
    const double mu     = b1.get_coef_frot_propre()      * b2.get_coef_frot_propre();
    const double lambda = calcul_lambda(alpha, m1, m2);
    
    bool  test_prof = test(alpha, mu, vitesse_normale, v_c);
    Vecteur delta_v = calcul_delta_v(test_prof, m1, m2, v_c, normal, lambda, mu, vitesse_normale);
    Vecteur delta_w = calcul_delta_w(test_prof, m1, m2, v_c, normal, lambda, mu, vitesse_normale, r1);
    
    return {delta_v, delta_w};
}

bool
mise_a_jour_apres_collision( Boule& b1, Boule& b2 )
{
    Vecteur x1 = b1.get_position();
    Vecteur x2 = b2.get_position();
    
    Vecteur v1 = b1.get_vitesse();
    Vecteur v2 = b2.get_vitesse();
    
    Vecteur w1 = b1.get_vit_angulaire();
    Vecteur w2 = b2.get_vit_angulaire();
    
    double  R1 = b1.get_rayon();
    double  R2 = b2.get_rayon();
    
    double  m1 = b1.get_masse();
    double  m2 = b2.get_masse();
    
    double distance = norme(x1 - x2);
    if( distance > R1 + R2 + 1e-5 ){
        // le almost_equal est très important pour éviter les erreurs numériques
        // /!\
        return false; // Pas de choc en cours
    }
    
    // alpha : coefficient de restitution qui indique la perte d'energie lors du choc
    //         [choc mou : 0 <= alpha <= 1 : choc elastique]
    double  alpha  = b1.get_coef_frot_restitution() * b2.get_coef_frot_restitution();
    
    // mu    : coefficient de frottement entre corps
    //         [0 <= mu]
    double  mu     = b1.get_coef_frot_propre() * b2.get_coef_frot_propre();
    
    double  lambda = (1+alpha)*m2 / (m1 + m2);
    
    // n     : vecteur normal au point de choc dirigé dans le corps 1
    //       : Il y a choc lorsque les corps partagent un point
    //         si l'un des deux est sphérique, alors "n" est orthogonal à la tangente
    //         i.e. "n" est donc dirigé vers le centre de la sphère
    Vecteur n  = (x1 - x2) / norme(x1 - x2);
    
    // composante normale de la vitesse
    double  v = produit_scalaire(v2 - v1, n);
    
    if(almost_equal(v, 0.0, 100)){
        //il y a beacoup d'erreur numérique qui s'accumulent lors du produit scalaire
        v = 0;
    }
    
    if(v < 0){
        //dans ce cas les objets s'écartent déjà
        //il n'y a pas collision
        return false;
    }
    
    // vitesse relative au point de contact :
    Vecteur Vc = v1 - v2 + v*n + produit_vectoriel(n, R1*w1 + R2*w2);
    
    // ces vecteurs seront modifiés et redimensionnés dans le if suivant
    Vecteur dV = {0};
    Vecteur dW = {0};
    
    if( 7. * mu * (1. + alpha) * v >= 2. * norme(Vc) ){
        dV = lambda * v * n - (  (2. * m2) / (7.*(m1 + m2))  ) * Vc;
        dW = (  (5. * m2) / (7.*R1*(m1 + m2))  ) * produit_vectoriel(n, Vc);
    }
    else{
        // Vc est forcément non-nul car [7. * mu * (1. + alpha)] * v est non nul
        Vecteur Vc_u = Vc / norme(Vc);
        dV = lambda * v * (n - mu * Vc_u);
        dW = (5. * mu * lambda * v / 2.*R1) * produit_vectoriel(n, Vc_u);
    }

    v1 = v1 + dV;
    w1 = w1 + dW;
    
    v2 = v2 - (m1 / m2) * dV;
    w2 = w2 + ( m1 * R1 / (m2 * R2) ) * dW;
    
    b1.set_vitesse(v1);
    b2.set_vitesse(v2);
    
    b1.set_vit_angulaire(w1);
    b2.set_vit_angulaire(w2);
    
    return true;
}

//****************************************************************************//
// Fonction collision Boule/Objets_immobile
//****************************************************************************//

double
calcul_lambda_ob_im(double alpha, double m1) // lambda = (1+alpha)*m2/(m1+m2)
{
    //la masse d'un objet n'est jamais nulle.
    return (1+alpha);
        // car convention m2/m1+m2 = 1;
}

Vecteur
calcul_delta_v_ob_im(bool test, double m1, Vecteur const& v_c, Vecteur const& n, double lambda, double mu, double v)
{
    if(test)
    {
        return (lambda * v * n)- (2./7.) * v_c;
    }
    else // Vc forcement non nul
    {
        if(DEBUGbool and norme(v_c) == 0){return (lambda * v * n)- (2./7.) * v_c;}
        return lambda * v * ( n - (mu * v_c/norme(v_c)));
    }
}

Vecteur
calcul_delta_w_ob_im(bool test, double m1, Vecteur const& v_c, Vecteur const& n, double lambda, double mu,
               double v, double r1)
{
    if(test)
    {
        return (5./(7. * r1))* produit_vectoriel(n, v_c);
    }else // Vc forcement non nul
    {
        if(DEBUGbool and norme(v_c) == 0){ return (5./(7. * r1))* produit_vectoriel(n, v_c);}
        return ((5. / 2.*r1)* mu * lambda * v) * produit_vectoriel(n, v_c/norme(v_c));
    }
}

Vecteur
calcul_v_relative_point_choc(const Boule& b, const ObstacleImmobile& o, Vecteur const& normal, double vit_normale)
{   // Vc = v1 - v2 + v*n + n ^ (r1w1)
    const Vecteur v1 = b.get_vitesse();
    const Vecteur w1 = b.get_vit_angulaire();
    const double  r1 = b.get_rayon();
    
    return v1 + vit_normale * normal + produit_vectoriel(normal, r1*w1);
    // on n'aditionne pas r2*w2 car r2 = 0
}



std::pair<Vecteur, Vecteur>
calcul_deltaV_deltaW(Boule const& b, ObstacleImmobile const& o)
{
    
    //const Vecteur ecart = b.get_position() - o.get_position();
    const Vecteur ecart = calcule_ecart(b, o);
    
    //const double norme_ecart = norme(ecart);
    //if (norme_ecart == 0) {
    //    return {0*b.get_vitesse(), 0*b.get_vit_angulaire()};
    // On utilise les methodes get pour avoir un vecteur de la bonne dimension
    //}
    
    const double m1 = b.get_masse();
    const double r1 = b.get_rayon();
    
    //const Vecteur normal = ecart / norme_ecart; //norme non nulle, test au dessus.
    const Vecteur normal_ = o.point_le_plus_proche_de(b.get_position()).normal_au_plan_tangent;
    const double  norme_normal = norme(normal_);
    if(norme_normal == 0){
        throw std::logic_error{"Impossible de calculer le rebond sans plan tangent"};
    }
    const Vecteur normal_unitaire = normal_ / norme_normal;
    
    
    
    const double  vitesse_normale = produit_scalaire(normal_unitaire, -b.get_vitesse() );
    const Vecteur v_c = calcul_v_relative_point_choc(b, o, normal_unitaire, vitesse_normale);
    
    const double alpha  = b.get_coef_frot_restitution() * o.get_coef_frot_restitution();
    const double mu     = b.get_coef_frot_propre()      * o.get_coef_frot_propre();
    const double lambda = calcul_lambda_ob_im(alpha, m1);
    
    bool  test_prof = test(alpha, mu, vitesse_normale, v_c);
    Vecteur delta_v = calcul_delta_v_ob_im(test_prof, m1, v_c, normal_unitaire, lambda, mu, vitesse_normale);
    Vecteur delta_w = calcul_delta_w_ob_im(test_prof, m1, v_c, normal_unitaire, lambda, mu, vitesse_normale, r1);
    
    return {delta_v, delta_w};
}

double assert_not_zero(double valeur, string message){
    if(valeur == 0){
        throw std::logic_error{message};
    }
    return valeur;
}

bool
mise_a_jour_apres_collision( Boule& b1, ObstacleImmobile const& o2 )
{
    
    Vecteur x1 = b1.get_position();
    
    auto zone_proche = o2.point_le_plus_proche_de(x1);
    Vecteur x2 = zone_proche.point;
    Vecteur v1 = b1.get_vitesse();
    Vecteur w1 = b1.get_vit_angulaire();
    double  R1 = assert_not_zero( b1.get_rayon(), "impossible de gérer une boule de rayon nul");
    
    if(norme( x1 - x2 ) > R1 + 1e-3){
        return false; // Pas de choc en cours
    }
    
    // alpha : coefficient de restitution qui indique la perte d'energie lors du choc
    //         [choc mou : 0 <= alpha <= 1 : choc elastique]
    double  alpha  = b1.get_coef_frot_restitution() * o2.get_coef_frot_restitution();
    
    // mu    : coefficient de frottement entre corps
    //         [0 <= mu]
    double  mu     = b1.get_coef_frot_propre() * o2.get_coef_frot_propre();
    
    double  lambda = (1+alpha);
    
    // n     : vecteur normal au point de choc dirigé dans le corps 1
    //       : Il y a choc lorsque les corps partagent un point
    //         si l'un des deux est sphérique, alors "n" est orthogonal à la tangente
    //         i.e. "n" est donc dirigé vers le centre de la sphère
    Vecteur n  = unitaire(zone_proche.normal_au_plan_tangent);//(x1 - x2) / assert_not_zero( norme(x1 - x2) , "impossible d'obtenir un vecteur normal au point de collision");
    
    // composante normale de la vitesse
    double  v = produit_scalaire(n, - v1);
    if(v < 0){
        return false;
        // Attention l'algorithme 1 d'évolution n'est pas toujours résilient face à cette réponse
    }

    // vitesse relative au point de contact :
    Vecteur Vc = b1.vitesse_au_point_de_contact(n);
    
    // ces vecteurs seront modifiés et redimensionnés dans le if suivant
    Vecteur dV = {0};
    Vecteur dW = {0};
    
    if( 7. * mu * (1. + alpha) * v >= 2. * norme(Vc) )
    {
        dV = lambda * v * n - (  2. / 7.  ) * Vc;
        dW = (  5. / (7.*R1)  ) * produit_vectoriel(n, Vc);
    }else
    { // Vc est forcément non-nul
        Vecteur Vc_u = Vc / assert_not_zero(norme(Vc), "erreur imprévue");
        dV = lambda * v * (n - mu * Vc_u);
        dW = (5. * mu * lambda * v / 2.*R1) * produit_vectoriel(n, Vc_u);
    }
     
    v1 = v1 + dV;
    w1 = w1 + dW;
    
    if(epsilon(v1, b1.get_vitesse()) and epsilon(w1, b1.get_vit_angulaire())){
        return false; // aucune modifications...
    }
    
    b1.set_vitesse(v1);
    b1.set_vit_angulaire(w1);
    
    return true;
}

bool
mise_a_jour_apres_collisions(Boule& b, Sol const& sol)
{
    return sol.reaction_normale(b, Sol::SEUIL_VITESSE_COLLISION)
    or mise_a_jour_apres_collision(b, static_cast<Paroi const&>(sol) );
    // (1) Nous tirons parti du fait que "or" est lazy en son second argument
    // (2) On slice le Sol volontairement, pour appeler la fonction(Boule&, ObctacleImmobile const&)
}

