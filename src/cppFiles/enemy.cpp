#include "enemy.h"
#include <SFML/Graphics.hpp>

    Enemy::Enemy(double x, double y, int size, sf::Color color)
    {
        this->x = x - size / 2;
        this->y = y - size / 2;
        this->color = color;
        this->size = size;
        this->radius = this->size / 2;
        this->speed = (defaultSpeed / radius);
    }

    void Enemy::drawEnemy(sf::RenderWindow &window)
    {
        sf::CircleShape enemy(radius);
        enemy.setPosition(x, y);
        enemy.setFillColor(color);
        window.draw(enemy);
    }

    void Enemy::setSpeed()
    {
        speed = (defaultSpeed / radius);
    }

    void Enemy::updateCenterCoordinates()
    {
        centerX = x + radius;
        centerY = y + radius;
    }

    void Enemy::moveEnemy(double sin, double cos)
    {
        this->x += cos * this->speed / 2;
        this->y += sin * this->speed / 2;
        updateCenterCoordinates();
    }


