//  Fichier :
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Copie d'une partie de la classe vecteur pour compiler l'exerice p03
//
#ifndef __projetEPFL__Vecteur__
#define __projetEPFL__Vecteur__

#include <valarray>
#include <ostream>

class Vecteur{
private:
    using Data_t = std::valarray<double>;
    
    Data_t data;
    Vecteur(Data_t&& data) :data( std::move(data) ) {}
 
public:
    static Vecteur DeTaille(size_t size );
    
    Vecteur(std::initializer_list<double>&& init );
    
    size_t size() const { return data.size(); }
    
    void push_back(double last_element);
    
    double& operator[] (size_t i) {return data[i]; }
    double const& operator[] (size_t i) const {return data[i]; }
    
    Vecteur& operator += (const Vecteur& autre);
    Vecteur& operator -= (const Vecteur& autre);
    
    Vecteur& operator *= (double scalaire) ;
    Vecteur& operator /= (double scalaire) ;
    Vecteur  operator -  () const;
    
    double*        begin ()       ;
    const double*  begin () const ;
    double*        end   ()       ;
    const double*  end   () const ;
};

// === fonctions complementaires ===
inline Vecteur operator + (Vecteur lhs, const Vecteur& rhs) {return lhs += rhs;}
inline Vecteur operator - (Vecteur lhs, const Vecteur& rhs) {return lhs -= rhs;}
inline Vecteur operator * (Vecteur lhs, double scalaire)    {return lhs *= scalaire;}
inline Vecteur operator * (double scalaire, Vecteur rhs)    {return rhs *= scalaire;}
inline Vecteur operator / (Vecteur lhs, double scalaire)    {return lhs /= scalaire;}

std::ostream& operator<<(std::ostream& flux, Vecteur const& v);
bool operator== (const Vecteur& lhs, const Vecteur& rhs);
bool operator!= (const Vecteur& lhs, const Vecteur& rhs);

double produit_scalaire(const Vecteur& lhs, const Vecteur& rhs);
double norme_carree(const Vecteur& vect);
double norme(const Vecteur& vect);
Vecteur produit_vectoriel(const Vecteur& lhs, const Vecteur& rhs);

double produit_mixte(const Vecteur& a, const Vecteur& b, const Vecteur& c);
Vecteur concatenation(Vecteur const& debut, Vecteur const& fin);

#endif /* defined(__projetEPFL__Vecteur__) */