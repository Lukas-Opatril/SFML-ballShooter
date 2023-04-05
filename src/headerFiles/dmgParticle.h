#pragma once
#include "particle.h"

class dmgParticle : public Particle
{
public:
    dmgParticle(double x, double y, int size, sf::Color shotColor, double directionX, double directionY);
    void shrinkParticle();

public:
    double RADIUS_DECREASE;
};
