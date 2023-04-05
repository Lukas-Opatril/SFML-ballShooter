#pragma once
#include "enemy.h"
#include "particle.h"
#include <SFML/Audio.hpp>

class enemyBoss : public Enemy
{
public:
	 int ID;
	enemyBoss(double x, double y, int size, sf::Color color,int ID);
	void shoot(double playerX, double playerY, std::vector<Particle> &particleArray);
};
