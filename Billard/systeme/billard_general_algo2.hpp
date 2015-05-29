//  Fichier : billard_general_algo2.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#include <iostream>
#include <numeric>
#include <iomanip>
#include <set>
#include <map>
#include <functional>
#include <algorithm>
#include "almost_equal.h"
#include "billard_general.h"
#include "integrateur.h"
#include "objet_integrable.h"
#include "obstacle_immobile.h"
#include "boule.h"
#include "sol.h"
#include "collision.h"

struct Collision{
    // c'est une std::pair<Objet*, Objet*> munit d'une relation d'ordre spécifique pour pointeurs
    less<Objet*> lt; //less<Objet*> est bien défini, alors que operator<(Objet*, Objet*) ne l'est pas
    Objet* first;
    Objet* second;
    Collision(Objet* a, Objet* b)
    : first{a}, second{b}
    {
        if ( lt(second, first) ) swap(second, first);
    }
    bool operator < (Collision const& c) const
    {
        return lt(first, c.first) or ( first == c.first and lt(second, c.second));
    }
};

struct WeakLt{
    bool operator()(double a, double b) const{
        return a < b and not almost_equal(a, b, 100);
    }
};

template <class DetecteurCollision, class GestionnaireCollision>
map<double, set<Collision>, WeakLt> BillardAlgo2_Base<DetecteurCollision, GestionnaireCollision>::

prochaines_collisions_avant_fin_du(double petit_dt)
{
    DetecteurCollision detecteur_collision(date_actuelle(), petit_dt, *integrateur() );
    map<double, set<Collision>, WeakLt> prochaines;
    
    for(auto mobile : objets_)
    {
        for(auto& immobile : memory_pool_)
        {
            if( objets_identiques( mobile, immobile.get() ) ){
                continue; //passe à la comparaison suivante
            }
            
            auto collision_possible = detecteur_collision.detecter(*mobile, *immobile);
            
            if(collision_possible.collision_trouvee)
            {
                // Nous attendons un dt dans l'intervalle [0;petit_dt]
                double prochain_dt = collision_possible.dt_avant_collision;
                prochaines[prochain_dt].emplace(mobile, immobile.get());
            }
        }
    }
    return prochaines;
}

template <class DetecteurCollision, class GestionnaireCollision>
bool BillardAlgo2_Base<DetecteurCollision, GestionnaireCollision>::
gerer_collision(std::set<Collision> const& collisions)
{
    // Ensuite, on gère chacune des collisions grâce à l'executeur de collision
    GestionnaireCollision executeur;
    
    bool at_least_one_done = false; //anglais car trop long en français
    
    for(auto const & collision : collisions){
        cout << "\t(x) paire en collision : " << endl;
        cout << "\t" << collision.first->uniq_id << " p1[" << collision.first->get_position() << "] v1[" << collision.first->get_vitesse() << "]" << endl;
        cout << "\t" << collision.second->uniq_id << " p2[" << collision.second->get_position() << "] v2[" << collision.second->get_vitesse() << "]" << endl;
        
        bool cur_done = executeur.executer(*collision.first, *collision.second);
        at_least_one_done = at_least_one_done or cur_done;
    }
    
    return at_least_one_done; //gérée correctement
}
template <class DetecteurCollision, class GestionnaireCollision>
void BillardAlgo2_Base<DetecteurCollision, GestionnaireCollision>::
evoluer(double petit_dt)
{
    //
    // Nouvel algorithme qui gère les faux positifs
    //
    if( almost_equal(0.0, petit_dt, 10) ){
        return;
    }
    cout << endl << endl;
    
    if( petit_dt < 1e-4){
        cout << "**********************************" << endl;
        cout << "méthode evoluer() " << endl;
        cout << "|  date actuelle :" << setprecision (15) << date_actuelle() << endl;
        cout << "|  le petit_dt est très petit :" << setprecision (15) << petit_dt << endl;
    }
    supprimer_les_objets_morts();
    
    for(auto mobile : objets_){
        using namespace evoluer_helper;
        
        arreter_si_trop_lent(*mobile);
        // Si la boule touche le sol : On recalcule les frottements
        // et on met à jour la résultante des forces
        sol_->reaction_de_frottement(*mobile);
        mobile->mise_a_jour_resultante_des_forces();
        
        // Si la boule touche le sol : On ajoute la reaction normale
        sol_->reaction_normale(*mobile);
    }
    
    
    auto prochaines = prochaines_collisions_avant_fin_du(petit_dt);
    // Nous attendons un tableau trié par dates croissantes
    // Nous attendons des prochaines[i].date dans [0 ; petit_dt]
    // Cela est compatible avec notre définition de collision (cf tests collision)
    
    if (prochaines.empty())
    {
        //cout << "-> aucune collision trouvée avant la fin du dt" << endl;
        faire_avancer_le_temps(petit_dt);
        
    }else
    {
        cout << "\n* Nous avons trouvé des collisions ce tour ci * " << endl;
        double petit_dt_restant = petit_dt;
        bool au_moins_une_collision_valide = false;
        
        // La boucle suivante cherche à diminuer le petit dt restant
        // Tant que nous n'avans pas géré de vraie collision
        for(auto prochaine : prochaines)
        {
            double dt_avant_collision = prochaine.first;
            auto const& objets_en_collision = prochaine.second;
            
            cout << "---- Nouvelle collision en cours de gestion : " << endl;
            cout << "\tNous avancons jusqu'à la date : "  << date_actuelle()
            << " + " << setprecision(10) << dt_avant_collision << endl;
            
            faire_avancer_le_temps(dt_avant_collision);
            petit_dt_restant -= dt_avant_collision;
            
            bool collision_valide = gerer_collision(objets_en_collision);
            
            if(collision_valide)
            {
                cout << "\tNous avons pu gérer la collision !" << endl;
                au_moins_une_collision_valide = true;
                break; //fin de la boucle car nous avons trouvé une vraie collision
            }else
            {
                cout << "\tNous n'avons pas pu gérer la collision..." << endl;
            }
        }//for
        
        // Si nous n'avons pas réussi à diminuer le dt_restant
        // C'est qu'il existe des faux positifs dans dt_avant_collision = 0
        // Dans ce cas, nous intégrons.
        if( not au_moins_une_collision_valide and almost_equal(petit_dt_restant, petit_dt, 10) ){
            cout << "\tFaux positifs ignorés : Nous intégrons de " << setprecision(10) << petit_dt_restant << endl;
            faire_avancer_le_temps(petit_dt_restant);
        }
        else{
            cout << "Fin de la gestion des collisions. Date actuelle : " << date_actuelle()
            << " dt_restant : " << setprecision(10) << petit_dt_restant << endl;
            
            evoluer(petit_dt_restant);
            // Algorithme essentiellement récursif... nous pourrons changer si problèmes de perf.
            // Idealement, le compilateur optimisera en tail-rec.
        }
    }//else
}