//
//
//
// IMPLEMENTATION DE LA FONCTION DE COLLISION EN COURS
//


#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include "vecteur.h"
#include "almost_equal.h"

#include "integrable.h"
#include "integrateur.h"
#include "trinome_second_degre.h"

#include "boule.h"
#include "objet.h"
#include "obstacle_immobile.h"

#include "detection_collision.h"
#include "calculateur_ecart.h"

using namespace std ;

//
// Cas Boule / Boule
//
bool
depassement(Vecteur const& ecart1, Vecteur const& ecart2)
{
    return produit_scalaire(ecart1, ecart2) < 0;
}

vector<double>
extraire_elements_entre_0_inclu_et_1_inclu(vector<double> const& original)
{
    vector<double> elements_valides;
    
    for(auto element : original)
        if( 0 <= element && element <= 1)
            elements_valides.push_back(element);
    
    return elements_valides;
}

std::pair<double,bool>
dt_jusque_collision(Boule & objet1, Boule & objet2,
                    double temps_depart, double dt_recherche_max, Integrateur& inte)
{
    std::pair<double, bool> JAMAIS{numeric_limits<double>::infinity(), false};
    
    Boule boule1 = objet1;
    Boule boule2 = objet2;
    inte.integre(boule1, temps_depart, dt_recherche_max);
    inte.integre(boule2, temps_depart, dt_recherche_max);
    
    const Vecteur x1 = objet1.get_position();
    const Vecteur x2 = objet2.get_position();
    const Vecteur xp1 = boule1.get_position();
    const Vecteur xp2 = boule2.get_position();
    const Vecteur dx = x2 - x1;
    const Vecteur dxp = xp2 - xp1;
    
    //exclusion des cas qui ne sont pas des collisions
    Vecteur n  = (x1 - x2);
    Vecteur vit1 = objet1.get_vitesse();
    Vecteur vit2 = objet2.get_vitesse();
    double  v = produit_scalaire(vit2 - vit1, n);
    if( v < 0){
        return JAMAIS;
    }
    
    //créer l'equation
    const double somme_des_rayons = objet1.get_rayon()+ objet2.get_rayon();
    const double a = norme_carree (dxp - dx);
    const double b = 2 * produit_scalaire(dx , (dxp - dx));
    const double c = norme_carree (dx) - std::pow(somme_des_rayons,2);
    
    vector<double> solutions_valides = ([&](){
        //demander les solutions
        vector<double> solutions = trinome::solutions_du_trinome(a , b, c); //ax^2 + bx + c = 0
        
        //prendre une décision en fonction des solutions
        return extraire_elements_entre_0_inclu_et_1_inclu(solutions);
    })();
    
    switch ( solutions_valides.size() )
    {
        case 0:
            return JAMAIS;
        case 1:
            return {dt_recherche_max * solutions_valides.front(), true};
        case 2:
            return {dt_recherche_max * std::min(solutions_valides.front()  ,  solutions_valides.back()), true};
        default:
            throw std::logic_error{"Il ne peut y avoir que 0, 1, ou 2 solutions !"};
    }
}


double
distance_entre(Vecteur const& v1, Vecteur const& v2)
{
    return norme(v1 - v2);
}

double
distance_entre(Boule const& b1, Boule const& b2)
{
    double distance_centres = distance_entre(b1.get_position(), b2.get_position());
    double distance_reelle = distance_centres - b1.get_rayon() - b2.get_rayon();
    return std::max(0. , distance_reelle);
}

bool
se_choquent(const Boule& b1, const Boule& b2) // ||x1-x2|| <= r1 + r2
{
    return distance_entre(b1, b2) <= 0;
}

//
// cas Boule / ObstacleImmobile
//

//***************************************************************************

Vecteur calcule_ecart(Boule const& b, Vecteur const & point_obstacle)
{
    //return b.point_le_plus_proche_de(point_obstacle).point - point_obstacle;
    
    // ecart : OB
    Vecteur ecart_au_centre = b.get_position() - point_obstacle;
    double norme_ecart_centre = norme(ecart_au_centre);
    
    if(norme_ecart_centre <= b.get_rayon()){
        // l'obstacle est rentré dans la boule, ou touche la boule
        return Vecteur{0,0,0};
    }
    else{
        //On diminue cet écart du rayon de la boule.
        double norme_ecart_boule  = norme_ecart_centre - b.get_rayon();
        return norme_ecart_boule * unitaire(ecart_au_centre);
    }
}

/*bool
collision(Vecteur const& ecart)
{
    return norme_carree(ecart) <= 1e-7;
}*/

double
dt_prochaine_collision_avec_ecarts(Vecteur const& ecart, Vecteur const& ecart_p, Vecteur const& n, double rayon, double dt_max)
{
    //Cas d'erreur : renvoit inf si les deux ecarts sont nuls et/ou si ils sont orthogonaux a n
    
    const double numerateur   = rayon - produit_scalaire(n, ecart);
    const double denominateur = produit_scalaire(n, ecart_p - ecart);
    const double tau = numerateur / denominateur;
    const double dt_collision = dt_max * tau;
    return dt_collision;
}

std::pair<double,bool>
dt_jusque_collision(ObstacleImmobile & o2, Boule const& boule1,
                    double date_actuelle, double dt_max,
                    Integrateur& inte)
{
    return dt_jusque_collision(boule1, o2, date_actuelle, dt_max, inte);
}

bool vitesse_nulle(Boule const& b1)
{
    return almost_equal(norme_carree(b1.get_vitesse()), 0.0, 10);
}
double distance_entre(Boule const& b1, Vecteur const& point){
    return max(0.0, distance_entre(b1.get_position(), point) - b1.get_rayon());
}
std::pair<double,bool>
dt_jusque_collision0(Boule const& b1, ObstacleImmobile & o2,
                    double date_actuelle, double dt_max,
                    Integrateur& inte)
{
    const std::pair<double, bool> PAS_DE_COLLISION{numeric_limits<double>::infinity(), false};
    //
    // Dans cet algorithme nous faisons la supposition que la vitesse ne va pas changer
    // i.e. que la vitesse initiale est "correcte"
    // Dans la conception du billard telle que décrite par l'énoncé, cette supposition est valide
    //
    Vecteur vitesse1 = b1.get_vitesse();
    if (zero(vitesse1)){
        return PAS_DE_COLLISION;
    }
    
    PointEtPlan zone_proche1 = o2.point_le_plus_proche_de( b1.get_position() );
    Vecteur normal1  = zone_proche1.normal_au_plan_tangent;
    
    if(zero(normal1)){
        throw std::logic_error{"Impossible d'obtenir un vecteur normal pour detecter la collision"};
    }
    
    if( produit_scalaire(vitesse1, normal1) >= 0){
        // la boule s'écarte de l'objet
        return PAS_DE_COLLISION;
    }
    
    // A ce stade nous savons que la boule se dirige vers l'objet
    // Il y a collision potentielle
    // Nous allons assimiler l'objet au plan suivant : zone_proche1
    // Nous avons conscience que ce modèle n'est pas parfait
    
    Vecteur point_proche1 = zone_proche1.point;
    
    Vecteur ecart_boule_objet1 = calcule_ecart(b1, point_proche1);
    
    double projection = produit_scalaire(ecart_boule_objet1, normal1);
    if( almost_equal(projection, 0.0, 100) ){
        if( distance_entre(b1, point_proche1) <= 1e-3){
            // la boule est en contact et va dépasser l'objet
            return {0, true};
        }else{
            // cas particulier où la boule est à coté de la surface
            return PAS_DE_COLLISION;
        }
    }
    if (projection < 0.){
        // la boule a déjà dépassé l'objet
        // c'est trop tard
        return PAS_DE_COLLISION;
    }
    
    // Pour savoir si il y a effectuvement collision avant la fin du dt_max
    // nous intégrons
    
    Boule b1_bientot = b1;
    inte.integre(b1_bientot, date_actuelle, dt_max);
    
    Vecteur ecart_boule_objet2 = calcule_ecart(b1_bientot, point_proche1);
    
    if( produit_scalaire(ecart_boule_objet2, normal1) <= 0 + 1e-7){
        // Dans dt_max la boule sera en contact avec l'objet ou l'aura dépassé
        // Rappelons que l'obstacle est assimilé à la surface zone_proche1
        
        // Nous assimilons la boule à un point matériel de position p(t).
        // Notons p_i = p(0) la position initiale et p_f = p(1) la position finale
        Vecteur p_i = point_proche1 + ecart_boule_objet1;
        Vecteur p_f = point_proche1 + ecart_boule_objet2;
        
        // Nous supposons que la vitesse est constante
        // Il y a contact lorsque le point est sur le plan
        // c'est à dire lorsque < normal1 | p(t) > = 0
        // ici < | > désigne le produit scalaire euclidien
        double denominateur = produit_scalaire(normal1, p_f-p_i);
        
        if(almost_equal(denominateur, 0.0, 100)){
            // Ce cas ne devrait pas arriver
            // Nous faisons quand même le test pour prévenir une division par 0
            // Il s'agit du cas où l'objet ne se dirige pas vers l'obstacle
            return PAS_DE_COLLISION;
        }
        double t_collision = -produit_scalaire(normal1, p_i) / denominateur;
        
        // Il faut encore transformer ce temps de [0;1] en dt de [0;dt_max]
        if( 0 <= t_collision and t_collision <= 1){
            double dt_collision = dt_max * t_collision;
            
            // Nous vérifions que notre modèle est valide dans ce cas
            // Pour cela c'est simple : il suffit d'intégrer du dt trouvé
            Boule b3 = b1;
            inte.integre(b3, date_actuelle, dt_collision);
            PointEtPlan zone_proche3 = o2.point_le_plus_proche_de(b3.get_position());
            Vecteur normal3 = zone_proche3.normal_au_plan_tangent;
            
            bool suffisament_proche = distance_entre(b3, zone_proche3.point) <= 1e-3;
            bool se_rapproche_de_obstacle = produit_scalaire(vitesse1, normal3) >= 0;
            
            if( suffisament_proche and se_rapproche_de_obstacle){
                return {dt_collision, true};
            }else{
                // Notre modèle a fait des approximations trops grandes
                return PAS_DE_COLLISION;
            }
        }
    }
    
    return PAS_DE_COLLISION;
}
std::pair<double,bool>
dt_jusque_collision(Boule const& b1, ObstacleImmobile & o2,
                     double date_actuelle, double dt_max,
                     Integrateur& inte)
{
    const std::pair<double, bool> PAS_DE_COLLISION{numeric_limits<double>::infinity(), false};
    //
    // Dans cet algorithme nous ne faisons aucune supposition quant à la variation de la vitesse
    // C'est à dire que l'on ne peut pas se fier à la vitesse initiale b1.get_vitesse()
    // Puisqu'elle est susceptible de subir une discontinuité lors de l'intégration
    //
    // Nous reprenons l'algorithme 1 en tenant compte de ces considérations
    //
    
    // D'abord nous calculons les informations sur la boule :
    // Sa position future dans dt_max
    // Et sa vitesse, que nous interpollons à partir des deux positions
    Boule b1_bientot = ([&](){
        Boule nouveau = b1;
        inte.integre(nouveau, date_actuelle, dt_max);
        return nouveau;
    }) () ;
    
    Vecteur pos_initiale = b1.get_position();
    Vecteur pos_finale   = b1_bientot.get_position();
    Vecteur vitesse = (b1_bientot.get_position() - b1.get_position()) / dt_max;
    
    if ( zero(vitesse) ){
        // Si la vitesse extrapolée est nulle, il n'y aura pas collision
        
        return PAS_DE_COLLISION;
    }
    // Ensuite les informations sur l'obstacle
    // Comme dans l'algorithme 1,
    // Nous supposons qu'il s'agit d'une surface passant par le point proche 1
    PointEtPlan zone_proche1 = o2.point_le_plus_proche_de( pos_initiale );
    Vecteur normal  = zone_proche1.normal_au_plan_tangent;
    Vecteur point_proche = zone_proche1.point;
    
    if( zero(normal) ){
        throw std::logic_error{"Impossible d'obtenir un vecteur normal pour detecter la collision"};
    }
    
    if( produit_scalaire(vitesse, normal) >= 0){
        // la boule s'écarte de l'objet
        // ou alors la vitesse est perpendiculaire au vecteur normal
        // et donc il n'y aura pas collision
        return PAS_DE_COLLISION;
    }
    
    // A ce stade nous savons que la boule se dirige vers l'objet
    // Il y a collision potentielle
    // D'abord nous devons vérifier que la boule n'a pas déjà dépassé l'objet
    
    Vecteur ecart_initial = calcule_ecart(b1, point_proche);
    Vecteur ecart_final   = calcule_ecart(b1_bientot, point_proche);
    //Vecteur ecart_final = point_proche + ecart_initial + vitesse*dt_max;
    
    // Nous testons d'abord s'il y a contact entre la position initale et l'obstacle
    double projection = produit_scalaire(ecart_initial, normal);
    if( almost_equal(projection, 0.0, 100) ){
        if( distance_entre(b1, point_proche) <= 1e-3){
            // la boule est en contact et va dépasser l'objet
            return {0, true};
        }else{
            // cas particulier où la boule est à coté de la surface
            return PAS_DE_COLLISION;
        }
    }
    
    // Nous testons maintenant si la boule est bien du bon coté de l'objet
    if (projection < 0.){
        // la boule a déjà dépassé l'objet
        // c'est trop tard
        return PAS_DE_COLLISION;
    }
    
    // La boule est du bon coté et se rapproche de l'objet
    // Nous faisons les calculs pour trouver le temps de collision
    
    if( produit_scalaire(ecart_final, normal) <= 0 + 1e-7){
        // Nous assimilons la boule à un point matériel de position p(t).
        // Ce point n'est pas le centre de la boule
        // Mais il s'agit du point le plus proche de l'obstacle
        // Notons p_i = p(0) la position initiale et p_f = p(1) la position finale
        Vecteur p_i = point_proche + ecart_initial;
        Vecteur p_f = point_proche + ecart_final;
        
        // Nous supposons que la vitesse est constante
        // Il y a contact lorsque le point est sur le plan
        // c'est à dire lorsque < normal1 | p(t) > = 0
        // ici < | > désigne le produit scalaire euclidien
        double denominateur = produit_scalaire(normal, p_f - p_i);
        
        if(almost_equal(denominateur, 0.0, 100)){
            // Ce cas ne devrait pas arriver
            // Nous faisons quand même le test pour prévenir une division par 0
            // Il s'agit du cas où l'objet ne se dirige pas vers l'obstacle
            // Mais a une trajectoire parallèle à la surface
            return PAS_DE_COLLISION;
        }
        
        double t_collision = -produit_scalaire(normal, p_i) / denominateur;
        if ( t_collision < 0 or 1 < t_collision){
            // La prochaine collision est dans trop longtemps
            return PAS_DE_COLLISION;
        }
        
        // Il faut encore transformer ce temps de [0;1] en dt de [0;dt_max]
        double dt_collision = dt_max * t_collision;
        return {dt_collision, true};
    }
    
    return PAS_DE_COLLISION;
}

//***************************************************************************


std::pair<double,bool>
dt_jusque_collision2(Boule const& b0, ObstacleImmobile & o2,
                    double date_actuelle, double dt_max,
                    Integrateur& inte)
{
    const std::pair<double, bool> PAS_DE_COLLISION{numeric_limits<double>::infinity(), false};
    CalculateurDeEcart ecart;
    //
    // Cet algorithme est celui de l'énoncé
    //
    auto surface = o2.point_le_plus_proche_de(b0.get_position());
    Vecteur n = surface.normal_au_plan_tangent;
    
    Vecteur ecart0 = ecart.entre(b0, o2);
    double R = b0.get_rayon();
    
    Boule b1 = ([&](){
        Boule nouveau = b0;
        inte.integre(nouveau, date_actuelle, dt_max);
        return nouveau;
    }) () ;
    
    Vecteur ecart1 = ecart.entre(b1, o2);
    
    if(norme(ecart1) <  R or produit_scalaire(ecart0, ecart1) )
    {
        double tau_num = R - produit_scalaire(n, ecart0);
        double tau_den = produit_scalaire(n, ecart1 - ecart0);
        if( not almost_equal(tau_den, 0.0, 10) ){
            double tau = tau_num / tau_den;
            return {tau*dt_max, 0 <= tau and tau <= 1};
        }
        
    }
    return {numeric_limits<double>::infinity(), false};
}
