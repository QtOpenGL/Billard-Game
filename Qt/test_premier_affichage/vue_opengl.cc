#include <iostream>
#include <QVector3D>
#include <QRect>
#include <QColor>
#include <stdexcept>
#include <memory>
#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt des différents attributs
#include "glsphere.h"
#include "billard_to_qt.h"

#include "boule.h"
#include "paroi.h"
#include "sol.h"
#include "brique.h"

void VueOpenGL::
dessin()
{
   // Dessine le 1er cube (à l'origine)
  dessineCube();

  QMatrix4x4 matrice;
  // Dessine le 2e cube
  matrice.translate(0.0, 1.5, 0.0);
  matrice.scale(0.25);
  dessineCube(matrice);

  // Dessine le 3e cube
  matrice.setToIdentity();
  matrice.translate(0.0, 0.0, 1.5);
  matrice.scale(0.25);
  matrice.rotate(45.0, 0.0, 1.0, 0.0);
  dessineCube(matrice);
}

// ======================================================================
void VueOpenGL::
operator()(Boule & b)
{
    QMatrix4x4 matrice;

    dessineAxes(matrice); // dessine le repère principal
    const Vecteur position = b.get_position();
    matrice.translate(position[0], position[1], position[2]);

    const Vecteur etat = b.etat();
    const Vecteur rotation = {etat[3], etat[4], etat[5]};
    matrice.rotate(norme(rotation), rotation[1], rotation[2], rotation[3]);

    matrice.scale(b.get_rayon());
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice, 0.0, 0.0); // bleu
    //matrice.scale(1.5); // taille des axes (1.5 pour qu'ils dépassent un peu)
    //dessineAxes(matrice, false); // dessine (en blanc) les axes de la sphere
}

void VueOpenGL::
operator()(Paroi & p)
{
    QMatrix4x4 matrice;

    Quadrilatere<QVector3D> quad;
    quad.bottom_right_corner = vec3d(p.get_position());
    quad.top_right_corner    = quad.bottom_right_corner + vec3d(p.get_longueur() );
    quad.bottom_left_corner  = quad.bottom_right_corner + vec3d(p.get_largeur());
    quad.top_left_corner     = quad.top_right_corner    + vec3d(p.get_largeur());

    dessineQuadrilatere(matrice, quad);
}
void VueOpenGL::
dessineQuadrilatere(QMatrix4x4 const& point_de_vue, Quadrilatere<QVector3D> const& q)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    QColor vert_herbe(0,181,0);

    glBegin(GL_QUADS);
    prog.setAttributeValue(CouleurId, vert_herbe );
    prog.setAttributeValue(SommetId, q.bottom_left_corner);
    prog.setAttributeValue(SommetId, q.bottom_right_corner);
    prog.setAttributeValue(SommetId, q.top_right_corner);
    prog.setAttributeValue(SommetId, q.top_left_corner);
    glEnd();
}
void VueOpenGL::
operator()(Sol &)
{
    QMatrix4x4 matrice;
    // Dessine le 2e cube
    matrice.translate(0.0, 2, 0.0);
    matrice.scale(0.2);
    matrice.rotate(45.0, 0.0, 1.0, 0.0);
    dessineCube(matrice);
}
void VueOpenGL::
operator()(Brique& b)
{
    QMatrix4x4 matrice;

    Quadrilatere<QVector3D> quad;
    quad.bottom_left_corner = vec3d(b.get_position());
    quad.bottom_right_corner  = quad.bottom_left_corner + vec3d(b.get_largeur());//+ QVector3D{1,0,0};//vec3d(b.get_largeur());

    quad.top_left_corner    = quad.bottom_left_corner + vec3d(b.get_longueur() );//QVector3D{0,1,0};//vec3d(b.get_longeure() );
    quad.top_right_corner   = quad.bottom_right_corner + vec3d(b.get_longueur() );// QVector3D{0,1,0};//   + vec3d(b.get_largeur());

    Quadrilatere<QVector3D> quad_top = quad;
    quad_top.translate(   vec3d(b.get_hauteur())   );

    dessineQuadriedre(matrice,  {quad_top, quad}  );
}
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
}

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
  //glEnable(GL_CULL_FACE);

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
  matrice_vue.translate(0, 0.0, -23.0);
  matrice_vue.rotate(-90.0, 1.0, 0.0, 0.0); //place z comme étant l'altitude
  matrice_vue.rotate(25.0, 1.0, 0.0, 0.0); //regarde au dessus du tapis
}

// ======================================================================
void VueOpenGL::translate(double x, double y, double z)
{
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(x, y, z);
  matrice_vue = translation_supplementaire * matrice_vue;
}

// ======================================================================
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}

// ======================================================================
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
  prog.setAttributeValue(SommetId, -1.0, -1.0, +.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +.0);
  glEnd();
}

// ======================================================================
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue, double rouge, double vert, double bleu)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, rouge, vert, bleu);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}

// ======================================================================
void VueOpenGL::dessineAxes (QMatrix4x4 const& point_de_vue, bool en_couleur)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_LINES);

  // axe X
  if (en_couleur) {
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
  } else {
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  }
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 1.0, 0.0, 0.0);

  // axe Y
  if (en_couleur) prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 1.0, 0.0);

  // axe Z
  if (en_couleur) prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0); // bleu
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 1.0);

  glEnd();
}
