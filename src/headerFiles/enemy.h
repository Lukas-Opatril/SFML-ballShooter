#pragma once
#include<SFML/Graphics.hpp>
class Enemy
{
public:
    double x, y, speed, centerX, centerY, radius;
    sf::Color color;
    int size;
    static int defaultSpeed;
   

    Enemy(double x, double y, int size, sf::Color color);
    void drawEnemy(sf::RenderWindow &window);
    void setSpeed();
    void updateCenterCoordinates();
    void moveEnemy(double sin, double cos);                
};

