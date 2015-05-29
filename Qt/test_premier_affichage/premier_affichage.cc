#include <QApplication>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include "glwidget.h"
#include "polygon.h"
#include "billard.h"
#include "boule.h"
#include "paroi.h"
#include "brique.h"

using namespace std;

class DummyBillard : public Billard{
public:
    Vecteur largeur{12,0,0}, longueur{0,25,0};
    Quadrilatere<Vecteur> interieur{
        -.5 * largeur + +.5 * longueur,
        +.5 * largeur + +.5 * longueur,
        -.5 * largeur + -.5 * longueur,
        +.5 * largeur + -.5 * longueur,
    };
    Vecteur x{.5,0,0}, y{0,.5,0}, z{0,0,.5};

    Paroi p{interieur.bottom_left_corner , largeur, longueur, 1., 1.};
    vector<Brique> bandes{
        Brique{interieur.bottom_left_corner    , largeur,-y,z,1,1},
        Brique{interieur.bottom_left_corner-y  , x,longueur+2*y,z,1,1},
        Brique{interieur.bottom_right_corner-y , x,longueur+2*y,z,1,1},
        Brique{interieur.top_left_corner       , largeur,y,z,1,1}
    };

    Boule b{0.5,1,1,1};

    DummyBillard()
    {
        b.set_etat(  Vecteur{0,0,b.get_rayon() , 0,0,0}  );
    }

    void evoluer(double) override {
        Vecteur etat = b.etat();
        b.set_etat( etat + Vecteur{0,0,0 , 0,1,0});
    }

    void se_dessiner(Viewer& vue) override {
        for(auto& brique : bandes) vue.dessiner(brique);
        vue.dessiner(b);
        vue.dessiner(p);
    }
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
