#include "dmgParticle.h"

dmgParticle::dmgParticle(double x, double y, int size, sf::Color shotColor, double directionX, double directionY)
    : Particle(x, y, size, shotColor, directionX, directionY,NULL)
{
    RADIUS_DECREASE = 0.2;
}

void dmgParticle::shrinkParticle()
{
    if (radius - RADIUS_DECREASE > 1)
    {
        radius -= RADIUS_DECREASE;
    }
}
