#ifndef OBJET_IMPLEMENTATION_TRAIT
#define OBJET_IMPLEMENTATION_TRAIT

#include "vecteur.h"


template <class Base>
class Positionnable : public Base
{
    Vecteur position_{0,0,0};

public :
    Vecteur get_position() const
    {
        return position_;
    }
    void set_position(Vecteur const& nouveau)
    {
        position_ = nouveau;
    }
};



template <class Base>
class AvecForce : public Base
{
    Vecteur forces_{0,0,0};

public :
    Vecteur get_resultante_des_forces() const
    {
        return forces_;
    }
    void set_resultante_des_forces(Vecteur const& resultante)
    {
        forces_ = resultante;
    }
};

#endif // OBJET_IMPLEMENTATION_TRAIT

