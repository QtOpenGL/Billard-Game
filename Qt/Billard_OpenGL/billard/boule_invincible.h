#ifndef BOULEINVINCIBLE_H
#define BOULEINVINCIBLE_H


#include "boule_de_couleur.h"

class BouleInvincible : public BouleDeCouleur
{
public:
    using BouleDeCouleur::BouleDeCouleur;

    void set_vie(bool vie) override;
    bool en_vie() const override;
};

#endif // BOULEINVINCIBLE_H
