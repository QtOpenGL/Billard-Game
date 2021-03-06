#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include <stdexcept>
#include <memory>
#include "glwidget.h"

// ======================================================================
int GLWidget::nouveau_timer()
{
    return startTimer(20);
}

void GLWidget::initializeGL()
{
  vue.init();
  timerId = nouveau_timer();
}

// ======================================================================
void GLWidget::resizeGL(int width, int height)
{
  /* On commance par dire sur quelle partie de la 
   * fenêtre OpenGL doit dessiner.
   * Ici on lui demande de dessiner sur toute la fenêtre.
   */
  glViewport(0, 0, width, height);

  /* Puis on modifie la matrice de projection du shader.
   * Pour ce faire on crée une matrice identité (constructeur 
   * par défaut), on la multiplie par la droite par une matrice
   * de perspective.
   * Plus de détail sur cette matrice
   *     http://www.songho.ca/opengl/gl_projectionmatrix.html
   * Puis on upload la matrice sur le shader à l'aide de la
   * méthode de la classe VueOpenGL
   */
  QMatrix4x4 matrice;
  matrice.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
  vue.setProjection(matrice);
}

// ======================================================================
void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  billard.se_dessiner(vue);
}


// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  constexpr double petit_angle(5.0); // en degrés
  constexpr double petit_pas(1.0);

  switch (event->key()) {

  case Qt::Key_Left:
    vue.rotate(petit_angle, 0.0, -1.0, 0.0);
    break;

  case Qt::Key_Right:
    vue.rotate(petit_angle, 0.0, +1.0, 0.0);
    break;

  case Qt::Key_Up:
    vue.rotate(petit_angle, -1.0, 0.0, 0.0);
    break;

  case Qt::Key_Down:
    vue.rotate(petit_angle, +1.0, 0.0, 0.0);
    break;

    //Vers l'arriere
  case Qt::Key_PageUp:
  case Qt::Key_G:
    vue.translate(0.0, 0.0,  petit_pas);
    break;

    //Vers l'avant
  case Qt::Key_PageDown:
  case Qt::Key_R:
    vue.translate(0.0, 0.0, -petit_pas);
    break;

    //Vers la gauche
  case Qt::Key_H:
    vue.translate( -petit_pas, 0.0, 0.0);
    break;

    //vers la droite
  case Qt::Key_N:
    vue.translate(+petit_pas, 0.0, 0.0);
    break;

    //vers le haut
  case Qt::Key_C:
    vue.translate(0.0, petit_pas, 0.0);
    break;

    //vers le bas
  case Qt::Key_T:
    vue.translate(0.0,  -petit_pas, 0.0);
    break;

  case Qt::Key_U:
    vue.rotate(petit_angle, 0.0, 0.0, -1.0);
    break;

  case Qt::Key_E:
    vue.rotate(petit_angle, 0.0, 0.0, +1.0);
    break;

  case Qt::Key_Home:
    vue.initializePosition();
    break;

  case Qt::Key_Space:
    pause();
    break;
  };

  updateGL(); // redessine
}

// ======================================================================
void GLWidget::timerEvent(QTimerEvent* event)
{
  Q_UNUSED(event);

  double dt = chronometre.restart() / 1000.0;
  billard.evoluer(dt);

  updateGL();
}

// ======================================================================
void GLWidget::pause()
{
  if (timerId == AUCUN_TIMER) {
    // dans ce cas le timer ne tourne pas alors on le lance
    timerId = nouveau_timer();
    chronometre.restart();
  } else {
    // le timer tourne alors on l'arrête
    killTimer(timerId);
    timerId = AUCUN_TIMER;
  }
}
