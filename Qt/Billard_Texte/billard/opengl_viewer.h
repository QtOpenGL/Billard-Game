#ifndef OPENGLVIEWER_H
#define OPENGLVIEWER_H

#include <QColor>
#include "object_viewer.h"

class VueOpenGL;

class OpenGlViewer : public ObjectViewer
{
    VueOpenGL& vue;
public:
    OpenGlViewer(VueOpenGL& vue);
    ~OpenGlViewer();

    virtual void operator()(Boule &)  override;
    virtual void operator()(BouleDeCouleur &)  override;
    virtual void operator()(BouleMortelle &)  override;
    virtual void operator()(Paroi &)  override;
    virtual void operator()(Sol &)    override;
    virtual void operator()(Brique &) override;
    virtual void operator()(Trou&) override;
private:
    void dessine_boule(Boule &, QColor boule, QColor axes);

};

#endif // OPENGLVIEWER_H
