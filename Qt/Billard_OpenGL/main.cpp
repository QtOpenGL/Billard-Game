#include <QApplication>
#include <iostream>
#include <vector>
#include <cmath>

#include "glwidget.h"
#include "opengl_viewer.h"

#include "polygon.h"
#include "integrateur.h"
#include "billard_general.h"
#include "boule.h"
#include "paroi.h"
#include "brique.h"
#include "sol.h"
#include "trou.h"
#include "telecommande_boule.h"
#include "boule_mortelle.h"
#include "boule_invincible.h"
#include "boule_de_couleur_couleur.h"

#include "coefficients_physiques.h"
#include "billard_builder.h"

using namespace std;
using namespace CHOISI;

template <class T, class... Args>
std::unique_ptr<T> make(Args&&... args){
    return std::unique_ptr<T>{ new T(std::forward<Args>(args)... ) };
}
std::unique_ptr<Boule> ptr_boule(Boule boule){
    return make<Boule>(std::move(boule));
}


int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
    using namespace std;


  Vecteur O {0,0,0};
  Vecteur e1{1,0,0};
  Vecteur e2{0,1,0};
  Vecteur e3{0,0,1};
  double largeur = 1.27, longueur = 2.54;

  VueOpenGL vue;
  auto billard = billard3x3AvecTrousSansBrique<BillardAlgo2>();

  OpenGlViewer viewer(vue);
  GLWidget w{vue, viewer, *billard};

  //
  // Ajout des boules sous forme de triangle
  //
  auto ajouter_boule = [&](double x, double y, bool bleu){
      QColor couleur{nullptr};
      if(bleu){ couleur = QColor{0,0,255}; } //bleu
      else    { couleur = QColor{200,0,0}; } //rouge sombre

      Boule* b = new BouleDeCouleur(rayon_boule, masse_boule, fr_boule, res_boule, new Couleur{couleur, couleur});
      b->set_etat({x,y,rayon_boule , 0,0,0});
      billard->ajouter(std::unique_ptr<Boule>( b ));
    };
    double lig_max = 5;
    double ecart = 2*rayon_boule + 0.01;

    double x0 = largeur / 2.0 - ecart * lig_max / 2.0;
    double y0 = 5./6. * longueur - ecart * lig_max;
    bool bleu = true;
    for(int i_ligne = 1; i_ligne <= lig_max; ++ i_ligne)
    {
      for(int i_col = 1; i_col <= i_ligne; ++i_col){
          double x1 = x0 + (lig_max-i_ligne) * ecart / 2.0;
          bleu = not bleu;
          ajouter_boule(x1 + ecart*i_col, y0 + ecart*i_ligne, bleu);
      }
    }


    //
    // Ajout de la boule télécommandée (qui est invincible)
    //
    QColor blanc_transparent{255,255,255,150};
    Couleur* couleur = new Couleur{blanc_transparent, blanc_transparent};
    if( couleur == nullptr ){
        throw "lol";
    }
    Boule* b = new BouleInvincible(rayon_boule, masse_boule*2, fr_boule, res_boule, couleur);
    //Boule* b = new BouleMortelle(rayon_boule, masse_boule*2, fr_boule, res_boule);
    b->set_position({.5*largeur,.2*longueur,rayon_boule});
    b->set_vitesse(6*O);

    Telecommande commande(*b, &w);
    billard->ajouter(std::unique_ptr<Boule>( b ));

    //
    // C'est parti
    //
    w.show();
    w.pause();
    return a.exec();
}
