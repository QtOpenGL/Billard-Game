//  Fichier : trinome_second_degre.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Résolution d'une equation du second degre
//

#ifndef Billard_trinome_second_degre_h
#define Billard_trinome_second_degre_h

#include <vector>
#include <stdexcept>
namespace trinome {
    
    //
    // Prototype des fonctions exportées
    //
    
    std::vector<double> solution_equation_affine(double a, double b); //ax + b = 0
    std::vector<double> solutions_du_trinome(double a, double b, double c); //ax^2 + bx + c = 0
    
    
    //
    // Implémentation des fonctions
    // Pour une meilleure lecture, lire DE BAS EN HAUT
    //
    
    inline std::vector<double>
    solution_equation_affine(double a, double b) //ax+c = 0
    {
        if(a == 0){
            return {}; // c = 0
        }else{
            return {-b/a};
        }
    }
    
    inline double
    discriminant_du_trinome(double a, double b , double c) // ax^2 + bx + c
    {
        return b*b - 4*a*c;
    }
    
    inline int
    nombre_de_solutions_du_trinome(double a, double b, double c, double discriminant)
    {
        if( discriminant  < 0 ) return 0;
        if( discriminant == 0 ) return 1;
        else return 2;
    }
    
    inline double
    solution1_du_trinome(double a, double b, double c, double discriminant)
    {
        return ( -b -sqrt(discriminant) ) / (2*a);
    }
    
    inline double
    solution2_du_trinome(double a, double b, double c, double discriminant)
    {
        return ( -b +sqrt(discriminant) ) / (2*a);
    }
    
    inline std::vector<double>
    solutions_du_trinome(double a, double b, double c) //ax^2 + bx + c = 0
    {
        if(a == 0){
            return solution_equation_affine(b, c); //bx+c = 0
        }
        
        const double delta = discriminant_du_trinome(a, b, c);
        const int nb_solutions = nombre_de_solutions_du_trinome(a, b, c, delta);
        
        switch (nb_solutions) {
            case 0:     return {};
            case 1:     return {  solution1_du_trinome(a, b, c, delta)  };
            case 2:     return {  solution1_du_trinome(a, b, c, delta), solution2_du_trinome(a, b, c, delta)  };
            default:    throw std::logic_error{"Un trinome a toujours entre 0 et 2 solutions"};
        }
    }
}

#endif
