#ifndef VUEOPENGL_H
#define VUEOPENGL_H

#include <QGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>
#include "glsphere.h"
#include "polygon.h"

class VueOpenGL{
 public:

    // méthodes de (ré-)initialisation
    void init();
    void initializePosition();

    // méthodes set
    void setProjection(QMatrix4x4 const& projection)
        { prog.setUniformValue("projection", projection); }

    void translate(QVector3D translation);
    void rotate(double angle, QVector3D axe);

    // méthode utilitaire
    void dessineAxes(QMatrix4x4 const& point_de_vue = {} , QColor x = {}, QColor y = {}, QColor z = {});

    //void dessineCube(QMatrix4x4 const& point_de_vue = QMatrix4x4{} );
    //void dessineQuadriedre(QMatrix4x4 const& point_de_vue = QMatrix4x4{}, Quadriedre<QVector3D> const& q);

    //void dessineRectangle(QMatrix4x4 const& point_de_vue = {} );
    void dessineQuadrilatere(QMatrix4x4 const& point_de_vue, Quadrilatere<QVector3D> const& quad);

    void dessineSphere (QMatrix4x4 const& point_de_vue , QColor couleur = {1,1,1} );
    void dessineSegment(QMatrix4x4 const& point_de_vue , QVector3D A, QVector3D B, QColor couleur);
    void dessineCercle (QMatrix4x4 const& point_de_vue , QColor couleur);

    QMatrix4x4 const& to_vue_world() const;

private:
    QGLShaderProgram prog;   // Un shader OpenGL encapsulé dans une classe Qt
    QMatrix4x4 matrice_vue;  // Caméra
    QMatrix4x4 translations;
    GLSphere sphere;
};

#endif
