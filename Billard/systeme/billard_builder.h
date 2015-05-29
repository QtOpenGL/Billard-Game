//  Fichier : billard_builder.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Fonctions pour construire ou remplir un billard
//

#ifndef __Billard__billard_factory__
#define __Billard__billard_factory__

#include <vector>
#include <memory>
#include "vecteur.h"
#include "sol.h"
#include "brique.h"
#include "paroi.h"
#include "integrateur.h"
#include "trou.h"
 
template <class BillardSpecifique>
std::unique_ptr<BillardGeneral>
billard3x3()
{
    using namespace std;
    using namespace CHOISI;

    Vecteur O {0,0,0};
    Vecteur e1{1,0,0};
    Vecteur e2{0,1,0};
    Vecteur e3{0,0,1};
    double largeur = 1.27, longueur = 2.54, hauteur = .04, epaisseur = .1;
    //double d_trou = .12, r_trou = .06;
    //double ep = epaisseur;

    unique_ptr<Sol> sol{ new Sol(O,largeur*e1,longueur*e2,fr_sol, res_sol, roul_sol, glis_sol) };
    Vecteur normal = sol->normal_unitaire_a_la_paroi();

    unique_ptr<Integrateur> integrateur{ new Integrateur_Newmark };
    unique_ptr<BillardGeneral> billard{ new BillardSpecifique(std::move(sol), std::move(integrateur)) };

    vector<vector<Vecteur>> dimensions_briques{
        {O-epaisseur*e1-epaisseur*e2 , (epaisseur+largeur+epaisseur)*e1 , epaisseur*e2, hauteur*e3 },
        {O-epaisseur*e1+longueur*e2  , (epaisseur+largeur+epaisseur)*e1 , epaisseur*e2, hauteur*e3 },
        {O-epaisseur*e1              , epaisseur*e1                     , longueur*e2 , hauteur*e3 },
        {O+largeur*e1                , epaisseur*e1                     , longueur*e2 , hauteur*e3 }
    };
    for(auto& d : dimensions_briques){
        billard->ajouter( unique_ptr<Brique>{ new Brique(d[0], d[1], d[2], d[3], res_bandes, fr_bandes)});
    }

    return billard;
}

std::vector<std::vector<Vecteur>>
bande_avec_trou(Vecteur direction_bande,
                                        Vecteur pos_offset, Vecteur pos_offset_trou, Vecteur pos_offset_trou2,
                                        Vecteur hauteur, Vecteur largeur,
                                        double rayon_trou, double longueur_bande)
{
    using namespace std;
    double r_trou = rayon_trou, d_trou = 2*r_trou;
    double longueur = longueur_bande;
    
    vector<double> ordonnee_des_parties{
        0, 1.5*d_trou, longueur/2.0 - d_trou, longueur/2.0, longueur/2.0 + d_trou, longueur-1.5*d_trou, longueur/*fin*/
    };
    const int nb_parties = ordonnee_des_parties.size() - 1;
    
    vector<vector<Vecteur>> bande;
    for(int i_partie = 0; i_partie < nb_parties; ++i_partie){
        Vecteur pos = ordonnee_des_parties[i_partie] * direction_bande + pos_offset;
        double taille = ordonnee_des_parties[i_partie+1] - ordonnee_des_parties[i_partie];
        Vecteur lon = taille * direction_bande;
        bande.push_back({pos, largeur, lon, hauteur});
    }
    
    for(auto i : {0}){
        bande[i][0] += - pos_offset + pos_offset_trou2;
        bande[i][2] +=   pos_offset - pos_offset_trou2;
    }
    for(auto i : {3}){
        bande[i][0] += - pos_offset + pos_offset_trou;
        bande[i][2] +=   pos_offset - pos_offset_trou;
    }
    for(auto i : {2}){
        bande[i][2] += - pos_offset + pos_offset_trou;
    }
    for(auto i : {5}){
        bande[i][2] += - pos_offset + pos_offset_trou2;
    }
    return bande;
}

std::vector<std::vector<Vecteur>>
bande_sans_trou(Vecteur direction_bande, Vecteur pos_offset, Vecteur pos_offset_trou,
                                        Vecteur hauteur, Vecteur largeur,
                                        double rayon_trou, double longueur_bande)
{
    using namespace std;
    double r_trou = rayon_trou, d_trou = 2*r_trou;
    double longueur = longueur_bande;
    
    vector<double> ordonnee_des_parties{
        0, 1.5*d_trou, longueur-1.5*d_trou, longueur/*fin*/
    };
    const int nb_parties = ordonnee_des_parties.size() - 1;
    
    vector<vector<Vecteur>> bande;
    for(int i_partie = 0; i_partie < nb_parties; ++i_partie){
        Vecteur pos = ordonnee_des_parties[i_partie] * direction_bande + pos_offset;
        double taille = ordonnee_des_parties[i_partie+1] - ordonnee_des_parties[i_partie];
        Vecteur lon = taille * direction_bande;
        bande.push_back({pos, largeur, lon, hauteur});
    }
    
    bande.front()[0] += - pos_offset + pos_offset_trou;
    bande.front()[2] +=   pos_offset - pos_offset_trou;
    bande.back()[2] += - pos_offset + pos_offset_trou;
    
    return bande;
}

std::vector<std::vector<Vecteur>>
coin(Vecteur O, Vecteur e1, Vecteur e2, Vecteur e3, double hauteur, double ep, double f)
{
    using namespace std;
    return vector<vector<Vecteur>>{                        
         { O-(1+f)*ep*e1 , f*ep*(-e2+e1), ep*e1, hauteur*e3 }
         ,{ O-(1+f)*ep*e2 , ep*e2, f*ep*(-e1+e2), hauteur*e3 }
    };
}

template <class BillardSpecifique>
std::unique_ptr<BillardGeneral>
billard3x3AvecTrous()
{
    using namespace std;
    using namespace CHOISI;

    Vecteur O {0,0,0};
    Vecteur e1{1,0,0};
    Vecteur e2{0,1,0};
    Vecteur e3{0,0,1};
    double largeur = 1.27, longueur = 2.54, hauteur = .04, epaisseur = .1;
    double r_trou = .06, ep = epaisseur;

    unique_ptr<Sol> sol{ new Sol(O,largeur*e1,longueur*e2,fr_sol, res_sol, roul_sol, glis_sol) };
    Vecteur normal = sol->normal_unitaire_a_la_paroi();

    unique_ptr<Integrateur> integrateur{ new Integrateur_Newmark };
    unique_ptr<BillardGeneral> billard{ new BillardSpecifique(std::move(sol), std::move(integrateur)) };

    auto ajouter = [&](vector<vector<Vecteur>> const& v){
        for(auto& d : v){
            billard->ajouter( unique_ptr<Brique>{ new Brique(d[0], d[1], d[2], d[3], res_bandes, fr_bandes)});
        }
    };
    double f = .8;
    auto bande_gauche = bande_avec_trou(e2, -ep*e1, -2*ep*e1, -(1+f)*ep*e1, hauteur*e3, ep*e1, r_trou, longueur);
    auto bande_droite = bande_avec_trou(e2, largeur*e1, (largeur+ep)*e1, (largeur+f*ep)*e1, hauteur*e3, ep*e1, r_trou, longueur);
    auto bande_avant = bande_sans_trou(e1, O, -f*ep*e2, hauteur*e3, -ep*e2, r_trou, largeur);
    auto bande_arriere = bande_sans_trou(e1, (longueur+f*ep)*e2, (longueur+2*ep)*e2, hauteur*e3, -ep*e2, r_trou, largeur);
    
    ajouter(bande_gauche);
    ajouter(bande_droite);
    ajouter(bande_avant);
    ajouter(bande_arriere);
    
    ajouter(coin(O, e1, e2, e3, hauteur, ep,f));
    ajouter(coin(O+largeur*e1, e2, -e1,  e3, hauteur, ep,f));
    ajouter(coin(O+longueur*e2, -e2, e1,  e3, hauteur, ep,f));
    ajouter(coin(O+largeur*e1+longueur*e2, -e1, -e2,  e3, hauteur, ep, f));
    
    double tm = .9* r_trou;
    double te = -.3 *r_trou;
    vector<Vecteur> positions_trous{
        {-tm, longueur/2.0, 0},
        {largeur+tm, longueur/2.0, 0},
        
        {-te, -te , 0},
        {largeur+te, -te, 0},
        
        {-te, longueur+te, 0},
        {largeur+te, longueur+te , 0},
    };
    
    for(auto& pos_trou : positions_trous){
        billard->ajouter( unique_ptr<Trou>{ new Trou{pos_trou, 1.5*r_trou, normal}});
    }

    return billard;
}
std::vector<Paroi>
paroi_pour_la_brique(Vecteur position, Vecteur largeur, Vecteur longueur, Vecteur hauteur, double frot_rest, double frot_propre){
    return {
        Paroi(position, longueur, largeur, frot_rest, frot_propre), // vers le bas
        Paroi(position+hauteur, largeur, longueur, frot_rest, frot_propre), //vers le haut
        Paroi(position, hauteur, longueur, frot_rest, frot_propre), //vers la gauche
        Paroi(position+largeur, longueur, hauteur, frot_rest, frot_propre), //vers la droite
        Paroi(position, largeur, hauteur, frot_rest, frot_propre), //vers l'avant
        Paroi(position+longueur, hauteur, largeur, frot_rest, frot_propre), //vers l'arrière
    };
}
template <class BillardSpecifique>
std::unique_ptr<BillardGeneral>
billard3x3AvecTrousSansBrique()
{
    using namespace std;
    using namespace CHOISI;

    Vecteur O {0,0,0};
    Vecteur e1{1,0,0};
    Vecteur e2{0,1,0};
    Vecteur e3{0,0,1};
    double largeur = 1.27, longueur = 2.54, hauteur = .04, epaisseur = .1;
    double r_trou = .06, ep = epaisseur;
    double offset_sol = 0.05;
    Vecteur centre = O + e1 * (largeur/2.) + e2 * (longueur / 2.);
    
    unique_ptr<Sol> sol{ new Sol(O-offset_sol*(e1+e2),(largeur+2*offset_sol)*e1,(longueur+2*offset_sol)*e2,fr_sol, res_sol, roul_sol, glis_sol) };
    Vecteur normal = sol->normal_unitaire_a_la_paroi();
    
    unique_ptr<Integrateur> integrateur{ new Integrateur_Newmark };
    unique_ptr<BillardGeneral> billard{ new BillardSpecifique(std::move(sol), std::move(integrateur)) };
    
    double f = .8;

    //
    // CONSTRUCTION DES BANDES DU BILLARD
    //
    using BriqueDim = vector<Vecteur>;
    using BandeDim = vector<BriqueDim>;

    enum{gauche = 0, droite = 1, avant = 2, arriere = 3};
    vector<int> dirs{gauche, droite, avant, arriere};

    // Récupere les bandes construites avec des BriqueDim

    vector<BandeDim> bandes_dim{
        bande_avec_trou(e2, -ep*e1, -2*ep*e1, -(1+f)*ep*e1, hauteur*e3, ep*e1, r_trou, longueur)
        ,bande_avec_trou(e2, largeur*e1, (largeur+ep)*e1, (largeur+f*ep)*e1, hauteur*e3, ep*e1, r_trou, longueur)
        ,bande_sans_trou(e1, O, -f*ep*e2,  hauteur*e3, -ep*e2, r_trou, largeur)
        ,bande_sans_trou(e1, (longueur+ep)*e2, (longueur+(1+f)*ep)*e2, hauteur*e3, -ep*e2, r_trou, largeur)
    };

    // Ajoute les coins

    vector<BandeDim> coins_dim{
         coin(O,            e1,  e2,  e3, hauteur, ep, f)
        ,coin(O+largeur*e1, e2, -e1,  e3, hauteur, ep,f)

        ,coin(O+largeur*e1+longueur*e2, -e1,  -e2,  e3, hauteur, ep, f)
        ,coin(O+longueur*e2,            -e2,   e1,  e3, hauteur, ep,f)
    };

    vector<int> dir_coin = {gauche, avant, droite, arriere};

    for(size_t i_dir = 0; i_dir < dir_coin.size(); ++i_dir){
        auto& cur_coin_dim = coins_dim[i_dir];
        auto& bande_gauche = bandes_dim[ dir_coin[i_dir] ];
        auto& bande_droite = bandes_dim[ dir_coin[(i_dir+1)%dir_coin.size()] ];
        bande_gauche.push_back(cur_coin_dim[0]);
        bande_droite.push_back(cur_coin_dim[1]);
    }

    // Convertit en bandes construites avec des Parois

    auto to_paroi = [&](vector<Vecteur> const& dim_brique)
    {
        return
        paroi_pour_la_brique(dim_brique[0], dim_brique[1], dim_brique[2], dim_brique[3], res_bandes, fr_bandes);
    };
    auto to_parois = [&](vector<vector<Vecteur>> const& briques){
        vector<Paroi> parois;
        for(auto& brique_dim : briques){
            vector<Paroi> cur_parois = to_paroi(brique_dim);
            for(auto& paroi : cur_parois){
                parois.push_back(paroi);
            }
        }
        return parois;
    };
    using BriqueEnParois = vector<Paroi>;
    vector<BriqueEnParois> bandes; //Le vector de bandes construites avec des parois

    for(auto& bande_dim : bandes_dim){
        bandes.push_back(to_parois(bande_dim));
    }

    // Filtre les parois obtenues pour supprimer celles qui sont inutiles
    // Puis les ajoute

    auto filtrer = [&](vector<Paroi> const& parois_a_filtrer, Vecteur sens_a_garder){
        vector<Paroi> parois_ok;
        for(auto& paroi : parois_a_filtrer){
            if(produit_scalaire(sens_a_garder, paroi.normal_unitaire_a_la_paroi()) > 0){
                parois_ok.push_back(paroi);
            }
        }
        return parois_ok;
    };

    using VecteursOkPourBande = vector<Vecteur>;
    vector<VecteursOkPourBande> normaux_ok{
        {e1, e3} , {-e1, e3} , {e2, e3} , {-e2, e3}
    };

    auto ajouter_parois = [&](vector<Paroi> const& v){
        for(auto& p : v){
            billard->ajouter( unique_ptr<Paroi>{ new Paroi(p) } );
        }
    };

    for(int dir : dirs){
        for(auto& normal_ok : normaux_ok[dir]){
            auto parois_ok = filtrer(bandes[dir], normal_ok);
            ajouter_parois(parois_ok);
        }
    }


    double tm = .9* r_trou;
    double te = -.3 *r_trou;
    vector<Vecteur> positions_trous{
        {-tm, longueur/2.0, 0},
        {largeur+tm, longueur/2.0, 0},
        
        {-te, -te , 0},
        {largeur+te, -te, 0},
        
        {-te, longueur+te, 0},
        {largeur+te, longueur+te , 0},
    };
    
    for(auto& pos_trou : positions_trous){
        billard->ajouter( unique_ptr<Trou>{ new Trou{pos_trou, 1.5*r_trou, normal}});
    }
    
    return billard;
}

#endif /* defined(__Billard__billard_factory__) */
