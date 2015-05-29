#ifndef BOULE_TELECOMMANDEE_IMPL_H
#define BOULE_TELECOMMANDEE_IMPL_H

#include <QObject>

class Boule;
class GLWidget;
class QKeyEvent;

class Telecommande : public QObject
{
    Q_OBJECT
public:
    Telecommande(Boule& b, GLWidget* fenetre, QObject* parent = nullptr);
signals:
public slots:
    void keyPressEvent(QKeyEvent* event);
private:
    GLWidget* fenetre;
    Boule& b;
};

#endif // BOULE_TELECOMMANDEE_IMPL_H

