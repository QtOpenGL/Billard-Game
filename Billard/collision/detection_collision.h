#ifndef MACR_______DETECTION_COLLISISON_H__________MACR
#define MACR_______DETECTION_COLLISISON_H__________MACR

#include <limits>
#include <cmath>
#include <vector>
#include "vecteur.h"
#include "boule.h"
#include "objet.h"
#include "obstacle_immobile.h"
#include "integrable.h"
#include "integrateur.h"
#include "trinome_second_degre.h"

//****************************************************************************//
// cas Boule / Boule
//****************************************************************************//

// dt_jusque_collision
// Cette fonction cherche le temps de prochaine collision entre [temps_depart;temps_depart+dt_recherche_max]
// Et renvoit le dt de collision de la forme : 0,345 * dt_recherche_max
// @return : (double) dt_collision tel que : 0 <= dt_collision <= dt_recherche_max
// @return : (bool) trouvee tel que : trouvee == false si aucune collision trouvée, true sinon.
std::pair<double,bool>
dt_jusque_collision(Boule & objet1, Boule & objet2,
                    double temps_depart, double dt_recherche_max, Integrateur& inte);

double
distance_entre(Boule const& b1, Boule const& b2);

bool
se_choquent(const Boule& b1, const Boule& b2); // ||x1-x2|| <= r1 + r2
//****************************************************************************//
// cas Boule / ObstacleImmobile
//****************************************************************************//

Vecteur
calcule_ecart(Boule const& b, Vecteur const& point_proche);

inline Vecteur
calcule_ecart(Boule const& b, ObstacleImmobile const& o)
{
    return calcule_ecart(b, o.point_le_plus_proche_de(b.get_position()).point);
}

std::pair<double,bool>
dt_jusque_collision(Boule const & boule1, ObstacleImmobile & o2,
                    double date_actuelle, double dt_max,
                    Integrateur& inte);

std::pair<double,bool>
dt_jusque_collision(ObstacleImmobile & o2, Boule const& boule1,
                      double date_actuelle, double dt_max,
                      Integrateur& inte);

std::pair<double,bool>
dt_jusque_collision2(Boule const & boule1, ObstacleImmobile & o2,
                    double date_actuelle, double dt_max,
                    Integrateur& inte);


#endif //eof