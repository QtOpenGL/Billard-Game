//  Fichier :
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Premiere mise en place du graphisme
//  Ce fichier n'est pas final
//  Nous voulons tester ici la boucle d'évenement
//

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include "vue_opengl.h"
#include "objet.h"
#include "billard.h"

class GLWidget : public QGLWidget
{
public:
  GLWidget(VueOpenGL& vue, Billard& billard, QWidget* parent = nullptr)
    : QGLWidget{parent}, vue{vue}, billard{billard}
  {}
  virtual ~GLWidget() {}

private:
  // Les 3 méthodes clés de la classe QGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;
  void pause();

private:
  //attribut liés a la gestion
  int timerId;
  int nouveau_timer();
  static constexpr int AUCUN_TIMER = 0;
  QTime chronometre; // pour faire évoluer les objets avec le bon "dt"

private:
  //partie specifique au billard
  VueOpenGL& vue;
  Billard& billard;
};

#endif // GLWIDGET_H
