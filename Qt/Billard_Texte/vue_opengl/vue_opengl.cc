#include <iostream>
#include <QVector3D>
#include <QRect>
#include <QColor>
#include <vector>
#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt des différents attributs
#include "glsphere.h"
#include "billard_to_qt.h"

#include "boule.h"
#include "paroi.h"
#include "sol.h"
#include "brique.h"
using namespace std;

// ======================================================================
void VueOpenGL::init()
{
  prog.addShaderFromSourceFile(QGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QGLShader::Fragment, ":/fragment_shader.glsl");

  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("couleur", CouleurId);
  
  prog.link();
  prog.bind();

  /*
   * Le Test de profondeur permet de dessiner un objet à l'arrière-plan
   * partielement caché par d'autres objets.
   * 
   * Le Back-face culling consiste à ne dessiner que les face avec ordre 
   * de déclaration dans le sens trigonométrique.
   */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );

  sphere.initialize();
  initializePosition();
}

// ======================================================================
void VueOpenGL::initializePosition()
{
  // position initiale
  matrice_vue.setToIdentity();
  translations.setToIdentity();

  translate( {-.5,-1,-1} );
  rotate(35.0, {1.0, 0.0, 0.0}); //regarde au dessus du tapis
}

// ======================================================================
void VueOpenGL::translate(QVector3D v)
{
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(v);
  matrice_vue = translation_supplementaire * matrice_vue;
  translations = translation_supplementaire * translations;
}

// ======================================================================
void VueOpenGL::rotate(double angle, QVector3D axe)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, axe);
  matrice_vue = rotation_supplementaire * matrice_vue;
}
QMatrix4x4 const& VueOpenGL::to_vue_world() const{
    return matrice_vue;
}
// ======================================================================
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue, QColor couleur)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, couleur);
  sphere.draw(prog, SommetId);
}

// ======================================================================
void VueOpenGL::dessineAxes (QMatrix4x4 const& point_de_vue, QColor x, QColor y, QColor z)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_LINES);

  // axe X
  prog.setAttributeValue(CouleurId, x);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 1.0, 0.0, 0.0);

  // axe Y
  prog.setAttributeValue(CouleurId, y);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 1.0, 0.0);

  // axe Z
  prog.setAttributeValue(CouleurId, z);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 1.0);

  glEnd();
}

void VueOpenGL::dessineSegment
(QMatrix4x4 const& point_de_vue, QVector3D A, QVector3D B, QColor couleur)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, couleur);
    prog.setAttributeValue(SommetId, A);
    prog.setAttributeValue(SommetId, B);

    glEnd();
}

// ======================================================================

void VueOpenGL::
dessineQuadrilatere(QMatrix4x4 const& point_de_vue, Quadrilatere<QVector3D> const& q)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    std::vector<QColor> couleurs{
        {120,0,0} , {0,100,100} , {0,1,100} , {100,0,100}
    };
    glBegin(GL_QUADS);
    prog.setAttributeValue(CouleurId, couleurs[0] );
    prog.setAttributeValue(SommetId, q.bottom_left_corner);

    prog.setAttributeValue(CouleurId, couleurs[1] );
    prog.setAttributeValue(SommetId, q.top_left_corner);

    prog.setAttributeValue(CouleurId, couleurs[2] );
    prog.setAttributeValue(SommetId, q.top_right_corner);

    prog.setAttributeValue(CouleurId, couleurs[3] );
    prog.setAttributeValue(SommetId, q.bottom_right_corner);
    glEnd();
}

void VueOpenGL::dessineCercle (QMatrix4x4 const& point_de_vue,
                               QColor couleur)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, couleur);  // met la couleur

  glBegin(GL_TRIANGLE_FAN);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  static const unsigned int faces(30);
  for (unsigned int i(0); i <= faces; ++i) {
    prog.setAttributeValue(SommetId, cos(2.*i*M_PI/faces), 0.0, -sin(2.*i*M_PI/faces));
  }
  glEnd();
}






//
// DEAD CODE IS EVIL
//

/*
void VueOpenGL::dessineQuadriedre(QMatrix4x4 const& point_de_vue, Quadriedre<QVector3D> const& q)
{
    auto const& h = q.face_superieure;
    auto const& b = q.face_inferieure;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    QColor vert (0,150,0,150);

    glBegin(GL_QUADS);
    prog.setAttributeValue(CouleurId, vert );

    prog.setAttributeValue(SommetId, h.bottom_left_corner);
    prog.setAttributeValue(SommetId, h.bottom_right_corner);
    prog.setAttributeValue(SommetId, h.top_right_corner);
    prog.setAttributeValue(SommetId, h.top_left_corner);

    prog.setAttributeValue(SommetId, h.bottom_left_corner);
    prog.setAttributeValue(SommetId, h.top_left_corner);
    prog.setAttributeValue(SommetId, b.top_left_corner);
    prog.setAttributeValue(SommetId, b.bottom_left_corner);

    prog.setAttributeValue(SommetId, h.bottom_right_corner);
    prog.setAttributeValue(SommetId, h.top_right_corner);
    prog.setAttributeValue(SommetId, b.top_right_corner);
    prog.setAttributeValue(SommetId, b.bottom_right_corner);

    prog.setAttributeValue(SommetId, b.bottom_left_corner);
    prog.setAttributeValue(SommetId, b.bottom_right_corner);
    prog.setAttributeValue(SommetId, h.bottom_right_corner);
    prog.setAttributeValue(SommetId, h.bottom_left_corner);

    prog.setAttributeValue(SommetId, b.top_left_corner);
    prog.setAttributeValue(SommetId, b.top_right_corner);
    prog.setAttributeValue(SommetId, h.top_right_corner);
    prog.setAttributeValue(SommetId, h.top_left_corner);

    prog.setAttributeValue(SommetId, b.bottom_left_corner);
    prog.setAttributeValue(SommetId, b.bottom_right_corner);
    prog.setAttributeValue(SommetId, b.top_right_corner);
    prog.setAttributeValue(SommetId, b.top_left_corner);
    glEnd();
}// ======================================================================

void VueOpenGL::dessineCube (QMatrix4x4 const& point_de_vue)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_QUADS);
  // face coté X = +1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);

  // face coté X = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);

  // face coté Y = +1
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0); // bleu
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);

  // face coté Y = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 1.0); // cyan
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);

  // face coté Z = +1
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 0.0); // jaune
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);

  // face coté Z = -1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 1.0); // magenta
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);

  glEnd();
}
void VueOpenGL::dessineRectangle(QMatrix4x4 const& point_de_vue)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  QColor vert_herbe(0,181,0);

  glBegin(GL_QUADS);
  prog.setAttributeValue(CouleurId, vert_herbe );
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  glEnd();
}*/
