#ifndef MACR_______IMPL_COLLISISON_H__________MACR
#define MACR_______IMPL_COLLISISON_H__________MACR

#include <limits>
#include <cmath>
#include <vector>

#include "vecteur.h"
#include "boule.h"
#include "objet.h"
#include "obstacle_immobile.h"
#include "sol.h"


//****************************************************************************//
// Fonction collision Boule/Boule
//****************************************************************************//

bool
mise_a_jour_apres_collision( Boule& b1, Boule& b2 );


// Fonctions utilitaires (exportées pour les tests unitaires)

Vecteur
calcul_delta_v(bool test, double m1, double m2, Vecteur const& v_c, Vecteur const& n, double lambda, double mu, double v);

Vecteur
calcul_delta_w(bool test, double m1, double m2, Vecteur const& v_c, Vecteur const& n, double lambda, double mu,
               double v, double r1);

Vecteur
calcul_v_relative_point_choc(const Boule& b1, const Boule& b2, Vecteur const& normal, double vit_normale);

double
calcul_lambda(double alpha, double m1, double m2);

bool
test (double alpha, double mu, double vit_normale, Vecteur const& v_c);

std::pair<Vecteur, Vecteur>
calcul_deltaV_deltaW(Boule const& b1, Boule const& b2);

//****************************************************************************//
// Fonction collision Boule/Objets_immobile
//****************************************************************************//

double
calcul_lambda_ob_im(double alpha, double m1);

Vecteur
calcul_delta_v_ob_im(bool test, double m1, Vecteur const& v_c, Vecteur const& n, double lambda, double mu, double v);

Vecteur
calcul_delta_w_ob_im(bool test, double m1, Vecteur const& v_c, Vecteur const& n, double lambda, double mu,
                     double v, double r1);

bool
mise_a_jour_apres_collision( Boule& b, ObstacleImmobile const& o );

inline bool
mise_a_jour_apres_collision(ObstacleImmobile const& o ,  Boule& b)
{
    return mise_a_jour_apres_collision(b, o);
}

Vecteur
calcul_v_relative_point_choc(const Boule& b, const ObstacleImmobile& o, Vecteur const& normal, double vit_normale);

std::pair<Vecteur, Vecteur>
calcul_deltaV_deltaW(Boule const& b, ObstacleImmobile const& o);


//****************************************************************************//
// Fonction collision Boule/Sol
//****************************************************************************//

bool
mise_a_jour_apres_collisions(Boule& b, Sol const& sol);
#endif //eof