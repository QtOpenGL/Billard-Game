//
// Exemple d'utilisation simple de la bibliothèque MultipleDistpach
// Auteur : Julien Harbulot
//

#include <iostream>

//#include "cpp11compatibility.h"
#include "multiple_dispatch.h"

using namespace std;

//===========================================================
// Utilisation basique de la bibliothèque MultipleDispatch.
//
//
// Dans cet exemple, nous allons effectuer un double dispatch
//
// On possède une collection d' Animal& (Chat, Chien)
// On veut qu'il puissent se saluer entre eux en ayant des
//   comportements différents selon le type de l'attaquant
//   et de l'attaqué.
//
// Pour cela, on définit les comportements voulus dans une
//   classe ActionSaluer.
// Par exemple : ActionSaluer::operator()(Chat&, Chat&){}
//
// Ensuite on crée une fonction (ou une méthode) qui prend
//  en argument deux Animal& puis qui appelle le dispatcher,
//  et enfin l'action sur les types retrouvés.
//
// Pour pouvoir retrouver les bons types, il faut indiquer
// à la bibliotheque sur quelles familles on veut travailler.
//
// Une famille est constituée :
// - d'une interface de Visiteur (ex : AnimalVisitor)
// - d'une classe Mère (Animal)
// - de types dérivés (Chat, Chien)
//
// Par exemple, pour créer la famille Animaux :
//
// using Animaux
// = dispatch::Family<AnimalVisitor, Animal, Chat, Chien>;



//===========================================================
// Dans cette partie, tout est classique :
//
// On crée une hierarchie classique (avec pattern Visitor)
//    Chat et Chien héritent de Animal
//    Les classes implémentent : void accept(AnimalVisitor&)
//

class Animal;
class Chat;
class Chien;

class AnimalVisitor{
public:
    virtual void visit(Chat& c) = 0;
    virtual void visit(Chien& c) = 0;
};

class Animal{
public:
    virtual void accept(AnimalVisitor& v) = 0;
    virtual ~Animal(){}
};

class Chat : public Animal{
public:
    virtual void accept(AnimalVisitor& v){ v.visit(*this); }
    
    Chat() = default;
    void dire_bonjour(){ cout << "miaou" << " "; }
private:
    Chat(const Chat &) = delete;
    Chat(Chat &&) = delete;
    Chat & operator= (const Chat &) = delete;
    Chat & operator= (Chat &&) = delete;
};

class Chien : public Animal{
public:
    virtual void accept(AnimalVisitor& v){ v.visit(*this); }
    
    Chien() = default;
    void dire_bonjour(){ cout << "wafwaf" << " "; }
private:
    Chien(const Chien &) = delete;
    Chien(Chien &&) = delete;
    Chien & operator= (const Chien &) = delete;
    Chien & operator= (Chien &&) = delete;
};

//===========================================================
// Ici on utilise la bibliothèque :
//
// On crée une action qui nécessite un multiple dispatch
// On l'enregistre auprès d'un multiple dispatcher
// Nombre de lignes pour utiliser la bibliotheque : 2 lignes.

class ActionSaluer{
public:
    ActionSaluer() = default;
    
    void operator()(Chat& lhs, Chat& rhs) { lhs.dire_bonjour(); rhs.dire_bonjour(); cout << endl;}
    void operator()(Chat& lhs, Chien& rhs){ lhs.dire_bonjour(); rhs.dire_bonjour(); cout << endl;}
    void operator()(Chien& lhs, Chat& rhs){ lhs.dire_bonjour(); rhs.dire_bonjour(); cout << endl;}
    void operator()(Chien& lhs, Chien& rhs){ lhs.dire_bonjour(); rhs.dire_bonjour(); cout << endl;}
    
    void operator()(Animal& lhs, Animal& rhs){
        using animal = dispatch::Family<AnimalVisitor, Animal, Chat, Chien>;
        dispatch::execute(*this, animal(lhs), animal(rhs));
    }
private:
    ActionSaluer(const ActionSaluer &) = delete;
    ActionSaluer(ActionSaluer &&) = delete;
    ActionSaluer & operator= (const ActionSaluer &) = delete;
    ActionSaluer & operator= (ActionSaluer &&) = delete;
};

//===========================================================
// fonction main(), on crée des Animal& et on essaie le
// double dispatch.

int main()
{
    cout << "Debut." << endl;
    
    Chat chat;
    Chien chien;
    
    Animal& a_chat = chat;
    Animal& a_chien = chien;
    
    ActionSaluer saluer;
    saluer(a_chat, a_chat);
    saluer(a_chat, a_chien);
    saluer(a_chien, a_chat);
    saluer(a_chien, a_chien);
    
    cout << "Fin." << endl;
    return 0;
}
