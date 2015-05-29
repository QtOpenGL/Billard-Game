//  Fichier : vecteur.cpp
//  Projet  : Billard - EPFL MA 1
//  Groupe  : 102
//  Auteurs : Zoé Snijders, Julien Harbulot
//
//  Description : Implémentation class Vecteur mathématique de dimension variable
//
#include <valarray>
#include <numeric>
#include <stdexcept>
#include <ostream>

#include "almost_equal.h"
#include "vecteur.h"
using namespace std;


// ==========================
// some helper functions for error handling

std::length_error
taille_nulle()
{
    return length_error{"la dimension d'un vecteur ne peut etre nulle"};
}

std::length_error
tailles_incompatibles()
{
    return length_error{"les vecteurs doivent avoir la meme dimension"};
}

std::invalid_argument
division_par_zero()
{
    return std::invalid_argument{"division par zéro non autorisée"};
}

// ==========================
// here is the implementation

Vecteur
Vecteur::DeTaille(size_t size )
{
    if(size == 0){
        throw taille_nulle();
    }
    Vecteur v{ 0 };
    v.data.resize(size);
    std::fill(v.begin(), v.end(), 0);
    return v;
}

Vecteur::
Vecteur(std::initializer_list<double>&& init)
: data{ std::move(init) }
{
    if( data.size() == 0) {
        throw taille_nulle();
    }
}

void Vecteur::
push_back(double last_element)
{
    //
    // Cette fonction a une complexité en O(n)
    // Alors que le push_back de std::vector est on O(1) amorti
    // Néanmoins, nous ne nous servirons jamais de cette fonction
    // Et les std::valarray présentent des performances numériques qui nous semblent préférables.
    //
    const size_t new_size = data.size()+1;
    Data_t new_data( new_size );
    
    std::copy(std::begin(data), std::end(data), std::begin(new_data));
    
    const size_t last_index = new_data.size()-1;
    new_data[ last_index ] = last_element;
    
    std::swap(data, new_data);
}


Vecteur& Vecteur::
operator+= (const Vecteur& autre)
{
    if (data.size() != autre.data.size()){
        throw tailles_incompatibles();
    }
    data+=autre.data;
    return *this;
}
Vecteur& Vecteur::
operator-= (const Vecteur& autre)
{
    if (data.size() != autre.size()){
        throw tailles_incompatibles();
    }
    data-=autre.data;
    return *this;
}

Vecteur& Vecteur::
operator*= (double scalaire)
{
    data*=scalaire;
    return *this;
}
Vecteur& Vecteur::
operator/= (double scalaire)
{
    if(scalaire ==  0){
        //abort();
        throw division_par_zero();
    }
    data/=scalaire;
    return *this;
}

Vecteur  Vecteur::
operator-() const
{

    return Vecteur(-data);
}


double* Vecteur::
begin ()
{
    return std::begin(data);
}
const double* Vecteur::
begin () const
{
    return std::begin(data);
}
double* Vecteur::
end   ()
{
    return std::end(data);
}
const double* Vecteur::
end   () const
{
    return std::end(data);
}

bool
operator==(const Vecteur& lhs, const Vecteur& rhs)
{   //operator== est transitif.
    
    if(lhs.size() != rhs.size()){
        return false;
    }
    bool egaux = true;
    for(size_t index = 0; index < lhs.size(); ++index) {
        egaux &= lhs[index] == rhs[index];
    }
    return egaux;
}

bool
operator!=(const Vecteur& lhs, const Vecteur& rhs)
{
    return not (lhs == rhs);
}

bool
epsilon(const Vecteur& lhs, const Vecteur& rhs, unsigned long epsilon_)
{   //espilon comparaison n'est pas forcement transitive.
    
    if(lhs.size() != rhs.size()){
        return false;
    }
    bool proches = true;
    for(size_t index = 0; index < lhs.size(); ++index) {
        proches = proches and almost_equal(lhs[index], rhs[index], epsilon_);
    }
    return proches;
}

double
produit_scalaire(const Vecteur& lhs, const Vecteur& rhs)
{
    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), 0.0);
}

double
norme_carree(const Vecteur& vect)
{
    return produit_scalaire(vect, vect);
}

double
norme(const Vecteur& vect)
{
    return std::sqrt(norme_carree(vect));
}

Vecteur
produit_vectoriel(const Vecteur& lhs, const Vecteur& rhs)
{   // par defaut on restreint les vecteurs à la dimension 3,
    // sinon le produit vectoriel n'a pas de sens.
    // (codé à la main car plus efficace)
    
    return {lhs[1]*rhs[2]-lhs[2]*rhs[1],
            lhs[2]*rhs[0]-lhs[0]*rhs[2],
            lhs[0]*rhs[1]-lhs[1]*rhs[0]};
}

ostream&
operator<<(ostream& flux, Vecteur const& v)
{
    if(v.size() == 0) return flux; //ne devrait jamais arriver
    
    size_t last_place = v.size()-1;
    for(auto index = 0; index < last_place; ++index){
        flux << v[index] << ", ";
    }
    flux << v[last_place];
    return flux;
}

double produit_mixte(const Vecteur& a, const Vecteur& b, const Vecteur& c){
    return produit_scalaire(produit_vectoriel(a, b), c);
}
Vecteur concatenation(Vecteur const& debut, Vecteur const& fin){
    Vecteur resultat    = Vecteur::DeTaille(  debut.size() + fin.size()  );
    auto pos_milieu = std::copy(debut.begin(), debut.end(), resultat.begin());
    std::copy(fin.begin(), fin.end(), pos_milieu);
    return resultat;
}
Vecteur unitaire(Vecteur const& autre){
    double taille = norme(autre);
    if( not almost_equal(taille, 0.0, 100) ){
        return autre / taille;
    }
    return autre;
}
bool zero(Vecteur const& vect){
    return almost_equal(norme_carree(vect), 0.0, 100);
}


 
