#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include "vue_opengl.h"
#include "opengl_viewer.h"
#include "objet.h"
#include "billard.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:

  GLWidget(VueOpenGL& vue, OpenGlViewer& viewer, Billard& billard, QWidget* parent = nullptr)
    : QGLWidget{parent}, vue{vue}, viewer{viewer}, billard{billard}
  {}
  virtual ~GLWidget() {}
  void pause();

signals:
  void keyPressSignal(QKeyEvent* event);

private:
  // Les 3 méthodes clés de la classe QGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;

private:
  //attribut liés a la gestion
  int timerId;
  int nouveau_timer();
  static constexpr int AUCUN_TIMER = 0;
  QTime chronometre; // pour faire évoluer les objets avec le bon "dt"

private:
  //partie specifique au billard
public:
  VueOpenGL& vue;
private:
  OpenGlViewer& viewer;
  Billard& billard;
};

#endif // GLWIDGET_H
