//  Fichier :
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description :
//
//  Ce fichier teste simplement
//  Que la boucle d'évenement est bien faite
//  La console doit afficher :
//  Billard . evoluer()
//  Billard . se_dessiner()
//
#include <QApplication>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "glwidget.h"
#include "billard.h"
using namespace std;

class DummyBillard : public Billard{
public:
    void evoluer(double) override { cout << "Billard . evoluer()" << endl; }
    void se_dessiner(Viewer&) override { cout << "Billard . se_dessiner()" << endl; }
};

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  VueOpenGL vue;
  DummyBillard billard;

  GLWidget w{vue, billard};
  w.show();
  return a.exec();
}
