#ifndef BILLARD_TO_QT
#define BILLARD_TO_QT

#include <QVector3D>
#include "vecteur.h"

QVector3D vec3d(Vecteur const& v){
    return QVector3D(v[0], v[1], v[2]);
}

#endif // BILLARD_TO_QT

