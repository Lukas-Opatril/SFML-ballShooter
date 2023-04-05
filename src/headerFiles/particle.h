#pragma once
#include <SFML/Graphics.hpp>

class Particle
{
public:
    double x, y, centerX, centerY;
    double radius;
    int size;
    sf::Color shotColor;
    double directionX, directionY;
    int ID;


    Particle(double x, double y, int size, sf::Color shotColor, double directionX, double directionY,int ID);
    void drawParticle(sf::RenderWindow &window);
    void updateCenterCoordinates();
    void moveParticle(int FPS);
};
