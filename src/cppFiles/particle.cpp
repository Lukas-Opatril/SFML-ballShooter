#include "particle.h"

Particle::Particle(double x, double y, int size, sf::Color shotColor, double directionX, double directionY,int ID)
{
    this->x = x;
    this->y = y;
    this->shotColor = shotColor;
    this->directionX = directionX;
    this->directionY = directionY;
    this->size = size;
    this->radius = this->size / 2;
    this->ID = ID;
}

void Particle::drawParticle(sf::RenderWindow &window)
{
    sf::CircleShape circle(radius);
    circle.setFillColor(shotColor);
    circle.setPosition(x, y);
    window.draw(circle);
}

void Particle::updateCenterCoordinates()
{
    centerX = x + radius;
    centerY = y + radius;
}

void Particle::moveParticle(int FPS)
{
    x += directionX * 10;
    y += directionY * 10;
    updateCenterCoordinates();
}
