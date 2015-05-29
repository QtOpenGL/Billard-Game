//  Fichier : coefficients_physiques.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Ce fichier définit les coefficients physiques
//

#include <cmath>
namespace P11{
    // Nous notons ici les coefficients de l'exercice P11
    const double rayon_boule = .02625;
    const double masse_boule = .127;
    const double fr_boule = 0.2;
    const double res_boule = .99;
    
    const double res_sol = .4;
    const double fr_sol = .05;
    const double glis_sol = .1;
    const double roul_sol = .05;
    
    const double hauteur_bandes = .2;
    const double res_bandes = .9;
    const double fr_bandes = .15;
}
namespace REEL{
    // Nous avons pu trouver les coefficients réels qui s'appliquent lors d'une partie du billard
    // bb signifie boule-boule
    // bs signifie boule-sol
    // bp signifie boule-paroi
    // bq signifie boule-queue
    // Source :
    //http://billiards.colostate.edu/threads/physics.html
    const double bb_frt = .06;   //frottement entre deux boules
    const double bb_res = .96;   //restitution choc entre deux boules
    const double bs_res = .5;    //restitution choc boule sol
    const double bp_res = .9;    //restitution choc boule paroi
    const double bs_roul = .005; //opposition au roulement boule sol
    const double bs_glis = .2;   //glissement boule sol
    const double bq_frt = .6;    //frottement boule queue
    
    //http://en.wikipedia.org/wiki/Billiard_ball
    const double rayon_boule = .068;
    const double masse_boule = .162;
}
namespace CHOISI{ 
    // Nous calculons les coefficients de chaque objet
    // A partir des coefficients rééls
    // Ces calculs sont compatibles avec les formules utilisées par notre moteur physique
    //using namespace REEL;
    const double rayon_boule = REEL::rayon_boule;
    const double masse_boule = REEL::masse_boule;
    const double fr_boule = std::sqrt(REEL::bb_frt);
    const double res_boule = std::sqrt(REEL::bb_res);
    
    const double res_sol = REEL::bs_res / res_boule;
    const double fr_sol = REEL::bs_roul / fr_boule;
    const double glis_sol = REEL::bs_glis / fr_boule; //opposition au glissement
    const double roul_sol = REEL::bs_roul / fr_boule;
    
    const double hauteur_bandes = .2;
    const double res_bandes = REEL::bp_res / res_boule;
    const double fr_bandes = REEL::bs_roul / fr_boule;
}
void afficher_constantes(){
    // Nous gardons cette fonction ici car elle est utile
    // Néanmoins, pour éviter d'importer <iostream>, nous
    // La mettons en commentaire
    /*
    cout <<  "rayon : " << rayon_boule << endl;
    cout << "masse : " << masse_boule << endl;
    cout << "fr_boule : " << fr_boule << endl;
    cout << "res_boule : " << res_boule << endl;
    
    cout << "res_sol : " << res_sol << endl;
    cout << "fr_ol : " << fr_sol << endl;
    cout << "glis_sol : " << glis_sol << endl;
    cout << "roul_sol : " << roul_sol << endl;
    
    cout << "hauteur : " << hauteur_bandes << endl;
    cout << "res_bandes : " << res_bandes << endl;
    cout << "fr_bandes : " << fr_bandes << endl;
*/
}
#ifndef __Billard__coefficients_physiques__
#define __Billard__coefficients_physiques__



#endif /* defined(__Billard__coefficients_physiques__) */
