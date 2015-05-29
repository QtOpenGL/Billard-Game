#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
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
  resize(2*width(), 2*height());
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
  billard.se_dessiner(viewer);
}


// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  emit keyPressSignal(event);
  constexpr double petit_angle(2.0); // en degrés
  constexpr double petit_pas(.5);

  QVector3D x{1,0,0}, y{0,1,0}, z{0,0,1};

  switch (event->key()) {

  case Qt::Key_O:
    vue.rotate(-petit_angle, y);
    break;

  case Qt::Key_U:
    vue.rotate(petit_angle, y);
    break;

  case Qt::Key_Period:
    vue.rotate(-petit_angle, x);
    break;

  case Qt::Key_E:
    vue.rotate(petit_angle, x);
    break;

    //Vers le bas
  case Qt::Key_M:
    vue.translate(petit_pas * y);
    break;

    //Vers le haut
  case Qt::Key_G:
    vue.translate(petit_pas * -y);
    break;

    //Vers la gauche
  case Qt::Key_H:
    vue.translate( petit_pas * x);
    break;

    //vers la droite
  case Qt::Key_N:
    vue.translate(-petit_pas * x);
    break;

    //vers l'avant
  case Qt::Key_C:
    vue.translate(petit_pas * z);
    break;

    //vers l'arriere
  case Qt::Key_T:
    vue.translate(-petit_pas * z);
    break;

  case Qt::Key_A:
    vue.rotate(-petit_angle, z);
    break;

  case Qt::Key_I:
    vue.rotate(petit_angle, z);
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
    dt = 0.01;
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
