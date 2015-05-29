//  Fichier : newmark.h
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Implémentation de l'algorithme de Newmark avec indice max
//

#ifndef __Billard__newmark__
#define __Billard__newmark__

//
// Algorithme de Newmark
//
// f est la fonction d'évolution suivante :
// Omega''(t) = f(t,Omega(t),Omega'(t))
//
// où :
// Omega(t) est le vecteur position,
// Omega'(t)    le vecteur vitesse,
// Omega''(t)   le vecteur acceleration
//
// sachant que :
//   epsilon est l'ordre de convergence
//   iteration_max est le nombre maximal d'itérations à effectuer
//
// retour :
// La fonction renvoit { Omega(t+dt) , Omega'(t+dt) }
//
template <class F>
std::pair<Vecteur, Vecteur>
algorithme_newmark(Vecteur pos_0, Vecteur vit_0, F f, double t_0, double dt, double epsilon, unsigned iteration_max)
{
    // simplification d'écriture pour l'algorithme :
    // on se rapporte au cas d'une suite omega(n) et omega_p(n).
    
    std::array<Vecteur, 2> data_pos { pos_0 , pos_0 };
    std::array<Vecteur, 2> data_vit { vit_0 , vit_0 };
    
    auto pos = [&](int n) -> Vecteur& { return data_pos[ n% 2 ]; };
    auto vit = [&](int n) -> Vecteur& { return data_vit[ n% 2 ]; };
    auto t   = [&](int n) -> double   { return t_0 + n*dt; };
    
    const double dt_l_2  = dt / 2.0;
    const double dt2_l_3 = dt*dt / 3.0;
    const double epsilon2 = epsilon * epsilon;
    
    
    // initialisation de la suite
    pos(1) = pos(0) = pos_0;
    vit(1) = vit(0) = vit_0;
    Vecteur Xp = f( t(0) , pos(0) , vit(0) );
    
    // genere la suite des pos(n) et des vit(n)
    int n = 0;
    do{
        ++n;
        
        Vecteur X = f(t(n) , pos(n) , vit(n) );
        vit(n) = vit(n-1) + dt_l_2 * (X + Xp);
        pos(n) = pos(n-1) + dt * vit(n-1) + dt2_l_3 * (.5 * X + Xp);
    }while (norme_carree( pos(n)-pos(n-1) ) >= epsilon2 and n < iteration_max);
    
    return {pos(n) , vit(n)};
}


#endif /* defined(__Billard__runge_kutta__) */
