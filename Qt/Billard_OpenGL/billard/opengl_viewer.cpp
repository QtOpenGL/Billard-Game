#include <numeric>
#include <cmath>
#include <QMatrix3x3>
#include "opengl_viewer.h"
#include "vue_opengl.h"
#include "vecteur.h"
#include "boule.h"
#include "paroi.h"
#include "sol.h"
#include "brique.h"
#include "billard_to_qt.h"

#include "telecommande_boule.h"

#include "boule_de_couleur.h"
#include "boule_de_couleur_couleur.h"

#include "boule_mortelle.h"
#include "trou.h"

QMatrix4x4
rotation_pour(QVector3D antecedant, QVector3D image)
{
    antecedant.normalize();
    image.normalize();

    constexpr double RAD = 57.2957795;
    double angle    = std::acos(QVector3D::dotProduct(antecedant, image)) * RAD;
    QVector3D axe   = QVector3D::crossProduct(antecedant, image);

    QMatrix4x4 rotation;
    rotation.rotate(angle, axe);
    return rotation;
}



OpenGlViewer::OpenGlViewer(VueOpenGL &vue)
    :vue{vue}
{
}

OpenGlViewer::~OpenGlViewer()
{
}

void OpenGlViewer::
operator()(Boule& b)
{
    QColor bleu{0,0,255};
    QColor bleu_fonce{0,0,255};
    dessine_boule(b, bleu, bleu_fonce);
}

void OpenGlViewer::
operator()(BouleMortelle& b)
{
    dessine_boule(b, {0,70,0}, {0,70,0});
}
void OpenGlViewer::
operator()(BouleDeCouleur& b)
{
    if( b.couleur == nullptr){
        QColor blanc{255,255,255};
        dessine_boule(b, blanc, blanc);
    }else{
        dessine_boule(b, b.couleur->couleur_boule, b.couleur->couleur_axes);
    }
}
void OpenGlViewer::
dessine_boule(Boule& b, QColor boule, QColor axes)
{
    QMatrix4x4 matrice;

    QVector3D position = to_vec3d(  b.get_position() );
    matrice.translate(position);

    //QVector3D q_vit_angulaire = to_vec3d(b.get_vit_angulaire());
    //q_vit_angulaire /= 10.0;
    //vue.dessineSegment(matrice, -q_vit_angulaire, q_vit_angulaire, {255,255,255});

    QVector3D rotation = to_vec3d( b.get_rotation() );
    constexpr double DEG = 57.2957795; //1 rad in degree
    matrice.rotate(rotation.z()*DEG, 0,0,1);
    matrice.rotate(rotation.y()*DEG, 0,1,0);
    matrice.rotate(rotation.x()*DEG, 1,0,0);

    matrice.scale(b.get_rayon());

    vue.dessineAxes(matrice, axes, axes, axes);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    vue.dessineSphere(matrice, boule);
}

void OpenGlViewer::
operator()(Paroi & p)
{
    QMatrix4x4 matrice;

    QVector3D position = to_vec3d(p.get_position() );
    matrice.translate(position);

    //dessine la paroi
    Quadrilatere<QVector3D> quad;

    QVector3D longueur = to_vec3d(p.get_longueur() );
    QVector3D largeur  = to_vec3d(p.get_largeur()  );

    quad.bottom_right_corner = {0,0,0};
    quad.top_right_corner    = quad.bottom_right_corner + longueur;
    quad.bottom_left_corner  = quad.bottom_right_corner + largeur;
    quad.top_left_corner     = quad.top_right_corner    + largeur;

    vue.dessineQuadrilatere(matrice, quad);

    //dessine le vecteur normal :
    //matrice.setToIdentity();
    //matrice.translate(  position + .5 * (longueur + largeur)  );
    //vue.dessineSegment(matrice, {0,0,0}, to_vec3d(.1 * p.normal_unitaire_a_la_paroi()), {255,255,255});
}

void OpenGlViewer::
operator()(Sol & sol)
{
    Paroi& p = sol;
    this->operator ()(p);
}

void OpenGlViewer::
operator()(Brique& b)
{
    for(auto const& paroi : b.get_parois() ){
        // C'est la mise en oeuvre du pattern visitor
        // Qui nous oblige a utiliser ce procédé bien connu
        // Nous savons que l'objet ne sera pas modifié
        // Mais il nous est impossible d'utiliser le mot clef const
        // Donc nous le supprimons avec un const_cast
        this->operator ()(  const_cast<Paroi&>(paroi)  );
    }
}

void OpenGlViewer::
operator()(Trou& trou)
{
    QMatrix4x4 matrice;

    matrice.translate(to_vec3d(trou.get_position()));
    matrice.translate(QVector3D{0.0, 1e-3, 0.0});
    matrice.scale(trou.get_rayon());

    QVector3D normal = to_vec3d( trou.get_normal() );
    QVector3D vertical = to_vec3d( {0,0,1} );
    QMatrix4x4 rotation = rotation_pour(normal, vertical);
    matrice = matrice * rotation;

    QColor noir{0,0,0, 200};
    vue.dessineCercle(matrice, noir);
}
