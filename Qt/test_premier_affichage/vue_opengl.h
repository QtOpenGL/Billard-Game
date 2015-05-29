#ifndef VUEOPENGL_H
#define VUEOPENGL_H

#include <QGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <QVector3D>
#include "object_viewer.h"
#include "glsphere.h"
#include "polygon.h"

class VueOpenGL : public ObjectViewer{
 public:

    virtual void operator()(Boule &)  override;
    virtual void operator()(Paroi &)  override;
    virtual void operator()(Sol &)    override;
    virtual void operator()(Brique &) override;
    virtual void operator()(Trou&) override {}

    // méthodes de (ré-)initialisation
    void init();
    void initializePosition();

    // méthodes set
    void setProjection(QMatrix4x4 const& projection)
        { prog.setUniformValue("projection", projection); }

    void translate(double x, double y, double z);
    void rotate(double angle, double dir_x, double dir_y, double dir_z);

    // méthode utilitaire
    void dessineAxes     (QMatrix4x4 const& point_de_vue = QMatrix4x4{} , bool en_couleur = true);

    void dessineCube      (QMatrix4x4 const& point_de_vue = QMatrix4x4{} );
    void dessineQuadriedre(QMatrix4x4 const& point_de_vue = QMatrix4x4{}, Quadriedre<QVector3D> const& q = Quadriedre<QVector3D>{});

    void dessineRectangle(QMatrix4x4 const& point_de_vue = QMatrix4x4{} );
    void dessineQuadrilatere(QMatrix4x4 const& point_de_vue = QMatrix4x4{}, Quadrilatere<QVector3D> const& quad = Quadrilatere<QVector3D>{} );

    void dessineSphere   (QMatrix4x4 const& point_de_vue = QMatrix4x4{} , double rouge = 1.0, double vert = 1.0, double bleu = 1.0);

 private:
    QGLShaderProgram prog;  // Un shader OpenGL encapsulé dans une classe Qt
    QMatrix4x4 matrice_vue; // Caméra
    GLSphere sphere;
    void dessin();
};

#endif
