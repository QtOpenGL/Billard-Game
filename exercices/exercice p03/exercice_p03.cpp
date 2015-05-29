//  Fichier : exercice P1
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Nous reprenons le fichier de l'exerice p01 pour compiler le makefile
//                Ce n'est donc pas le contenu de ce fichier qui constitue l'exercice, mais bien celui du makefile
#include <iostream>
#include "vecteur.h"

using namespace std;

void structure_affichage()
{
    cout << endl;
    cout << "-----------------------------------------------------" << endl;
}

int main() {
    cout << "EXERCICE P03" << endl;
    
    const Vecteur v1 {1.0, 2.0, -0.1};
    const Vecteur v2 {2.6, 3.5, 4.1};
    const Vecteur v0 {0, 0, 0};
    
    //(1.0, 2.0, -0.1) + (2.6, 3.5, 4.1) = (3.6, 5.5, 4.0)
    structure_affichage();
    cout << "( " << v1 << " ) + ( " << v2 << " ) = ( " << v1+v2 << " ) " << endl;
    
    //(1.0, 2.0, -0.1) - (2.6, 3.5, 4.1) = (-1.6, -1.5, -4.2)
    cout << "( " << v1 << " ) - ( " << v2 << " ) = ( " << v1-v2 << " ) " << endl;
    
    //l'addition du vecteur nul (i.e. (0,0,0) pas le vecteur de dimension nulle !) ne fait rien
    //la soustraction d'un vecteur à lui-même donne le vecteur nul (de la même dimension !)
    structure_affichage();
    cout << "addition du vecteur nul n'as pas d'effets "<< endl;
    cout << "( " << v1 << " ) + " << "( " << v0 << " ) = ( " << v1 << " ) " <<  endl;
    
    
    //verifier le lancement d'une exeption
    //vérifiez votre choix pour les opérations entre vecteurs de dimensions différentes (cf question P1.2)
    structure_affichage();
    cout << "test de l'addition de vecteur de taille différente  " << endl;
    try
    {
        const Vecteur v3{1,1};
        const Vecteur v4 = v1+v3;
    }
    catch(std::length_error)
    {
        cerr << "Taille incompatible " << endl;
    }
    
    //l'opposé de (1.0, 2.0, -0.1) est (-1.0, -2.0, 0.1)
    structure_affichage();
    cout <<"l'opposé de " << endl;
    cout << "( " << v1 << " ) est " << "( " << -v1 << " ) " << endl;
    
    //l'addition d'un opposé est égal à la soustraction
    structure_affichage();
    cout << "l'addition d'un vecteur opposé est égale a la soustraction " << endl;
    cout << "( " << v1 << " ) + ( " << -v2 << " ) = ( " << v1-v2 << " ) " << endl;
    
    //3 * (1.0, 2.0, -0.1) = (3.0, 6.0, -0.3)
    structure_affichage();
    cout << "la multipliquation par un scalaire : " << endl;
    cout << "3( " << v1 << " )= ( " << 3*v1 << " ) " << endl;
    
    //(1.0, 2.0, -0.1) * (2.6, 3.5, 4.1) = 9.19
    cout << "le produit scalaire : " << endl;
    cout << "( " << v1 << " ) * ( " << v2 << " ) = " << produit_scalaire(v1, v2) << " " << endl;
    
    //(1.0, 2.0, -0.1) ^ (2.6, 3.5, 4.1) = (8.55, -4.36, -1.7)
    cout << "le produit vectoriel :" << endl;
    cout << "( " << v1 << " ) ^ ( " << v2 << " ) = ( " << produit_vectoriel(v1, v2) << " ) " << endl;
    
    //||(1.0, 2.0, -0.1)||^2 = 5.01
    //||(2.6, 3.5, 4.1)||^2 = 35.82
    cout << "la norme carrée " << endl;
    cout << "||" << v1 << "||^2 = " << norme_carree(v1) << endl;
    cout << "||" << v2 << "||^2 = " << norme_carree(v2) << endl;
    
    return 0;
}
