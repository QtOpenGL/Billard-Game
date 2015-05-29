#include <QObject>
#include <QVector3D>
#include <QKeyEvent>
#include "glwidget.h"
#include "boule.h"
#include "billard_to_qt.h"
#include "telecommande_boule.h"

Telecommande :: Telecommande(Boule& b, GLWidget* fenetre, QObject* parent)
        : QObject(parent)
        , b{b}
        , fenetre{fenetre}
    {
        QObject::connect(fenetre, SIGNAL(keyPressSignal(QKeyEvent*)), this, SLOT(keyPressEvent(QKeyEvent*)));
    }

void Telecommande ::
keyPressEvent(QKeyEvent* event)
{
    double pas = 5 * b.get_rayon();

    // Cette fonction considere les ordres dans le référentiel du monde (i.e. d'openGL)
    // C'est a dire que la fleche du haut ira toujours face a la camera, meme si elle est deplacee.

    auto update_as_in_view = [&](Vecteur offset){
        auto const& mat_vue = this->fenetre->vue.to_vue_world();
        QVector3D vitesse_vue = mat_vue * to_vec3d( b.get_vitesse() );
        vitesse_vue += to_vec3d(offset);
        b.set_vitesse(from_vec3d(mat_vue.inverted() * vitesse_vue));
    };

    // Cette fonction considere les ordres dans le référentiel de départ
    // C'est a dire que la fleche du haut va toujours dans le meme sens
    // Meme si la camera est deplacee

    auto update = [&](Vecteur offset){
        b.set_vitesse(b.get_vitesse() + offset);
    };

    switch (event->key()) {
    case Qt::Key_Up:    update({ 0, 1, 0}); break;
    case Qt::Key_Down:  update({ 0,-1, 0}); break;
    case Qt::Key_Left:  update({-1, 0, 0}); break;
    case Qt::Key_Right: update({ 1, 0, 0}); break;
    case Qt::Key_Shift: update({ 0, 0, 1}); break;
    }
}
