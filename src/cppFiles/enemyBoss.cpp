#include "enemyBoss.h"
#include <cmath>

enemyBoss::enemyBoss(double x, double y, int size, sf::Color color, int ID)
    : Enemy(x, y, size, color)
{
    this->ID = ID;
}

void enemyBoss::shoot(double playerX, double playerY, std::vector<Particle> &particleArray)
{
    int distanceX = playerX - centerX;
    int distanceY = playerY - centerY;
    double angle = atan2(distanceY, distanceX);
    double sine = std::sin(angle);
    double cosine = std::cos(angle);
    particleArray.emplace_back(Particle(centerX, centerY, this->radius, color, cosine, sine, this->ID));
}
