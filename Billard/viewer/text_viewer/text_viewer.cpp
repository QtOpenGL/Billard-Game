//  Fichier : text_viewer.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : 
//

#include <iostream>
#include "text_viewer.h"

#include "objet.h"
#include "boule.h"
#include "paroi.h"
#include "sol.h"
#include "brique.h"

using namespace std;

void TextViewer::
operator()(Boule& b)
{
    cout << b << endl;
}

void TextViewer::
operator()(Paroi& p)
{
    //cout << "Affichage d'une paroi..." << endl;
    // Nous n'affichons rien car cela encombre pour la lecture
}

void TextViewer::
operator()(Sol&)
{
    //cout << "Affichage du sol..." << endl;
    // Nous n'affichons rien car cela encombre pour la lecture
}

void TextViewer::
operator()(Brique&)
{
    //cout << "Affichage d'une brique..." << endl;
    // Nous n'affichons rien car cela encombre pour la lecture
}
void TextViewer::
operator()(Trou&)
{
    //cout << "Affichage d'un trou..." << endl;
    // Nous n'affichons rien car cela encombre pour la lecture
}
