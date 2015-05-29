#ifndef VUEOPENGL_H
#define VUEOPENGL_H

//  Fichier :
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//  Premiere mise en place du graphisme :
//  La vue openGl pourra dessiner des objets sur l'écran
//

#include <QGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include "object_viewer.h"

class VueOpenGL : public ObjectViewer{
 public:

    virtual void operator()(Boule &)  override;
    virtual void operator()(Paroi &) override;
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
    void dessineCube(QMatrix4x4 const& point_de_vue = QMatrix4x4() );

 private:
    QGLShaderProgram prog;  // Un shader OpenGL encapsulé dans une classe Qt
    QMatrix4x4 matrice_vue; // Caméra
    void dessin();
};

#endif
