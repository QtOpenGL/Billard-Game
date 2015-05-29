//  Fichier : billard_general.cpp
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
#include "objet_mobile.h"
#include "obstacle_immobile.h"
#include "boule.h"
#include "sol.h"
#include "collision.h"

using namespace std;

//fonction utilitaire pour la gestion des erreurs
template <class T, class E>
void throw_if_nullptr(unique_ptr<T> const& ptr, E&& exception)
{
    if( ptr.get() == nullptr ){
        throw exception;
    }
}

BillardGeneral::
BillardGeneral(unique_ptr<Sol> nouveau_sol, unique_ptr<Integrateur> integrateur)
: integrateur_(move(integrateur))
{
    throw_if_nullptr(integrateur_, IntegrateurNulException{});
    throw_if_nullptr(nouveau_sol, ObjetNulException{});
    
    sol_ = nouveau_sol.release();
    obstacles_.push_back( sol_ );
    memory_pool_.emplace_back( sol_ );
    index_du_sol_dans_memory_pool_ = memory_pool_.size()-1;
}

BillardGeneral::BillardGeneral(BillardGeneral&& autre)
: integrateur_( move(autre.integrateur_))
, sol_         ( autre.sol_                    )
, memory_pool_ ( move(autre.memory_pool_ ))
, objets_     ( move(autre.objets_     ))
, obstacles_  ( move(autre.obstacles_  ))
{
    autre.sol_ = nullptr;
    autre.integrateur_.release();
}

BillardGeneral& BillardGeneral::
operator=(BillardGeneral&& autre)
{
    this->integrateur_= move(autre.integrateur_);
    this->memory_pool_ = move(autre.memory_pool_);
    this->objets_     = move(autre.objets_);
    this->obstacles_  = move(autre.obstacles_);
    this->sol_         = move(autre.sol_);
    return *this;
}

void BillardGeneral::
ajouter(unique_ptr<ObstacleImmobile> o){
    throw_if_nullptr(o, ObjetNulException{});
    
    obstacles_.push_back( o.get() ); //aucune gestion memoire
    memory_pool_.emplace_back( o.release() ); //gestion memoire
}

void BillardGeneral::
ajouter(unique_ptr<Boule>  o){
    throw_if_nullptr(o, ObjetNulException{});
    
    objets_.push_back( o.get() ); //aucune gestion memoire
    memory_pool_.emplace_back( o.release() ); //gestion memoire
}

void BillardGeneral::
integrateur(unique_ptr<Integrateur> nouveau)
{
    throw_if_nullptr(nouveau, IntegrateurNulException{});
    integrateur_ = move(nouveau);
}

void BillardGeneral::
se_dessiner(Viewer& viewer)
{
    for (auto& obstacle : obstacles_) {
        viewer.dessiner(*obstacle);
    }
    for (auto& objet : objets_) {
        viewer.dessiner(*objet);
    }
    // Note : par précondition les objets et les obstacles ne sont pas nullptr
}

void BillardGeneral::
faire_avancer_le_temps(double dt_integrateur_voulu)
{
    if(dt_integrateur_voulu < 0)
    {
        throw std::logic_error{"Impossible de faire remonter le temps"};
    }
    if(dt_integrateur_voulu > 0)
    {
        for (auto& p_objet : objets_) { //auto& si smart_prt pour eviter copie
            integrateur_->integre(*p_objet, date_actuelle_, dt_integrateur_voulu);
        }
        date_actuelle_ += dt_integrateur_voulu;
    }
}

bool
objets_identiques(Objet* o1, Objet* o2)
{
    // Note :
    // Cette méthode est intéressante grâce au slicing :
    // Lorsque nous l'appelerons avec un pointeur Boule* et un pointeur Objet*
    // Le pointeur Boule* sera converti par le compilateur en pointeur Objet*
    // Et nous pouvons donc comparer deux pointeurs vers Objet
    //
    // Il est important d'utiliser le slicing car pour un même objet
    // il peut arriver que le pointeur vers Boule
    // ne soit pas le même que le pointeur vers Objet
    // C'est souvent le cas lorsque l'on utilise l'héritage multiple.
    return o1 == o2;
}
void BillardGeneral::
supprimer_les_objets_morts()
{
    // Les deux lignes suivantes pourront être combinées avec le mot clef auto en c++14
    auto objet_mort       = [](Objet* objet){ return not objet->en_vie(); };
    auto smart_objet_mort = [&](unique_ptr<Objet> const& objet){ return objet_mort(objet.get()); };
    
    objets_.erase(remove_if(objets_.begin(), objets_.end(), objet_mort), objets_.end());
    memory_pool_.erase(remove_if(memory_pool_.begin(), memory_pool_.end(), smart_objet_mort), memory_pool_.end());
}

namespace evoluer_helper {
    bool vitesse_nulle(Vecteur const& v){ return norme(v) <= BillardGeneral::SEUIL_VITESSE_LINEAIRE; }
    bool vitesse_angulaire_nulle(Vecteur const& v){ return norme(v) <= BillardGeneral::SEUIL_VITESSE_ANGULAIRE; }
    bool resultante_forces_nulle(Vecteur const& v){ return norme(v) <= BillardGeneral::SEUIL_FORCE; }
    
    void arreter_si_trop_lent(Objet& mobile)
    {
        if( vitesse_nulle( mobile.get_vitesse() )
        and vitesse_angulaire_nulle( mobile.get_vit_angulaire() )
        and resultante_forces_nulle(mobile.get_resultante_des_forces()))
        {
            mobile.set_vitesse({0,0,0});
            mobile.set_vit_angulaire({0,0,0});
            mobile.set_resultante_des_forces({0,0,0});
        }
    }
}




//=====================================================
//                      BillardAlgo1
//=====================================================









namespace algo1_collision {
    
    using Executeur = ExecuteurDeCollision;
    using Detecteur = DetecteurDeCollision;
    
}

struct ProchaineCollision{
    vector<pair<Objet*, Objet*>> objets_en_collision;
    double dt_avant_collision;
    bool trouvee;
    
    ProchaineCollision(double dt, bool trouvee)
    : dt_avant_collision{dt}, trouvee{trouvee}
    {}
    
    void ajouter(Objet* objet1, Objet* objet2)
    {
        // Les paires d'objets sont ordonnées pour faciliter la gestion
        less<Objet*> lt;
        if( lt(objet1, objet2) ){
            objets_en_collision.emplace_back( objet1, objet2 );
        }
        else{
            objets_en_collision.emplace_back( objet2, objet1 );
        }
    }
    void unique()
    {
        auto pair_lt = [](pair<Objet*, Objet*>& a, pair<Objet*, Objet*>& b){
            less<Objet*> lt;
            return lt(a.first, b.first) or (a.first == b.first and lt(a.second, b.second));
        };
        
        sort(objets_en_collision.begin(), objets_en_collision.end(), pair_lt);
        
        auto nouvelle_fin = std::unique(objets_en_collision.begin(), objets_en_collision.end());
        objets_en_collision.erase(nouvelle_fin, objets_en_collision.end());
    }
    bool operator<(ProchaineCollision const& autre) const{
        double dt_b = autre.dt_avant_collision;
        return dt_avant_collision < dt_b and not almost_equal(dt_avant_collision, dt_b, 10);
    }
};
ProchaineCollision BillardAlgo1::
prochaine_collision_avant_fin_du(double petit_dt) //petit_pas <= dt_integrateur_
{
    algo1_collision::Detecteur detecteur_collision(date_actuelle(), petit_dt, *integrateur() );
    ProchaineCollision   prochaine_collision(numeric_limits<double>::infinity(), false);
    
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
                // Nous attendons un dt dans l'intervalle [0 ; petit_dt]
                
                double dt_avant_collision = collision_possible.dt_avant_collision;
                
                if(almost_equal(dt_avant_collision, prochaine_collision.dt_avant_collision, 10))
                { //si la nouvelle collision a lieu à la même date, on la sauvegarde
                    prochaine_collision.ajouter(mobile, immobile.get());
                }
                if(dt_avant_collision < prochaine_collision.dt_avant_collision)
                { //on a detecté une collision plus proche, donc on remplace
                    prochaine_collision = ProchaineCollision(dt_avant_collision, true);
                    prochaine_collision.ajouter(mobile, immobile.get());
                    
                    // detecteur_collision.dt_recherche_max = prochaine_collision.date - date_actuelle_;
                    // mise à jour du dt_max
                    // il nous semble que cet micro-optimisation est inutile...
                }
            }
        }
    }
    return prochaine_collision;
}

bool BillardAlgo1::
gerer_collision(ProchaineCollision&& p) //&& pour forcer un move (rvalue reference)
{
    // D'abord on s'assure de ne pas gérer deux fois la même collision
    p.unique();
    
    // Ensuite, on gère chacune des collisions grâce à l'executeur de collision
    algo1_collision::Executeur executeur;
    bool collision_gere = false;
    for(auto& collision : p.objets_en_collision){
        bool courante_gere = executeur.executer(*collision.first, *collision.second);
        collision_gere = collision_gere or courante_gere;
    }
    return collision_gere;
}

void BillardAlgo1::
evoluer(double petit_dt)
{
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
    
    ProchaineCollision prochaine = prochaine_collision_avant_fin_du(petit_dt);
    // Nous attendons une prochaine.date dans [0 ; petit_dt]
    // Cela est compatible avec notre définition de collision (cf tests collision)
    
    if (not prochaine.trouvee) {
        faire_avancer_le_temps(petit_dt);
        
    }else{
        faire_avancer_le_temps(petit_dt);
        
        gerer_collision(  move(prochaine)  );
        double petit_dt_restant = petit_dt - prochaine.dt_avant_collision; // >= 0
        evoluer(petit_dt_restant);
        // Algorithme essentiellement récursif... nous pourrons changer si problèmes de perf.
        // Idealement, le compilateur optimisera en tail-rec.
    }
}





//=====================================================
//                      BillardAlgo2
//=====================================================






namespace algo2_collision {
    using Executeur = ExecuteurDeCollision;
    using Detecteur = DetecteurDeCollision;
}
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

map<double, set<Collision>, WeakLt> BillardAlgo2::
prochaines_collisions_avant_fin_du(double petit_dt)
{
    algo2_collision::Detecteur detecteur_collision(date_actuelle(), petit_dt, *integrateur() );
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

bool BillardAlgo2::
gerer_collision(std::set<Collision> const& collisions)
{
    // Ensuite, on gère chacune des collisions grâce à l'executeur de collision
    algo2_collision::Executeur executeur;
    
    bool at_least_one_done = false; //anglais car trop long en français
    
    for(auto const & collision : collisions){
        bool cur_done = executeur.executer(*collision.first, *collision.second);
        at_least_one_done = at_least_one_done or cur_done;
    }
    
    return at_least_one_done; //gérée correctement
}
void BillardAlgo2::
evoluer(double petit_dt)
{
    //
    // Nouvel algorithme qui gère les faux positifs
    //
    if( almost_equal(0.0, petit_dt, 10) ){
        return;
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
        faire_avancer_le_temps_securise(petit_dt);
    }else
    {
        double petit_dt_deja_parcouru = 0;
        bool au_moins_une_collision_valide = false;
        
        // La boucle suivante cherche à diminuer le petit dt restant
        // Tant que nous n'avans pas géré de vraie collision
        for(auto prochaine : prochaines)
        {
            double dt_avant_collision = prochaine.first - petit_dt_deja_parcouru;
            
            if(dt_avant_collision < 0){
                cout << "correction d'un dt invalide" << endl;
                continue; // Cette collision est invalide
            }
            auto const& objets_en_collision = prochaine.second;
            
            faire_avancer_le_temps_securise(dt_avant_collision);
            petit_dt_deja_parcouru += dt_avant_collision;
            
            bool collision_valide = gerer_collision(objets_en_collision);
            
            if(collision_valide)
            {
                au_moins_une_collision_valide = true;
                break; //fin de la boucle car nous avons trouvé une vraie collision
            }
        }//for
        
        double petit_dt_restant = petit_dt - petit_dt_deja_parcouru;
        
        // Si nous n'avons pas réussi à diminuer le dt_restant
        // C'est qu'il existe des faux positifs dans dt_avant_collision = 0
        // Dans ce cas, nous intégrons.
        if( not au_moins_une_collision_valide and almost_equal(petit_dt_deja_parcouru, 0., 10) ){
            faire_avancer_le_temps_securise(petit_dt_restant);
        }
        else{
            evoluer(petit_dt_restant);
            // Algorithme essentiellement récursif... nous pourrons changer si problèmes de perf.
            // Idealement, le compilateur optimisera en tail-rec.
        }
    }//else
}
void BillardAlgo2::
faire_avancer_le_temps_securise(double dt)
{
    faire_avancer_le_temps(dt);
    for(auto mobile : objets_){
        sol_->ajuster_position(*mobile);
    }
}
