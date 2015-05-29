#ifndef BILLARD_TO_QT
#define BILLARD_TO_QT

#include <QVector3D>
#include "vecteur.h"

//
// Ce fichier est tres important puisqu'il permet de faire
// la conversion entre les conventions du billard (axe z vers le haut)
// et celles de Qt et d'OpenGl (axe y vers le haut)
//
//
inline QVector3D
to_vec3d(Vecteur const& v)
{
    return QVector3D(v[0], v[2], -v[1]);
}
inline Vecteur
from_vec3d(QVector3D const& v)
{
    return Vecteur{v[0], -v[2], v[1]};
}

//
// On vérifie que to_vec3d(from_vec3d( v )) == v
// v{a,b,c}
// v1 = from_vec3d(v) <=> v1{a,-c,b}
// v2 = to_vec3d(from_vec3d(v)) <=> v2{a,b,--c} <=> v2{a,b,c}
//
#endif // BILLARD_TO_QT

