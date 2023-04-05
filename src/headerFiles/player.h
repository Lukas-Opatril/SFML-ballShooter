#pragma once
#include <SFML/Graphics.hpp>
#include "floorBall.h"
#include "particle.h"
#include <vector>

class Player
{
public:
    double x, y;
    float centerX, centerY;
    double moveX = 0;
    double moveY = 0;
    static int radius;
    static int size;
    sf::Color color;
    static int DMG;
    std::vector<sf::CircleShape> circleArray;
    unsigned short int numberOfShots;
    unsigned short int shotRadius;
    unsigned int angleStep;
    Player(double x, double y, sf::Color color, const unsigned short int numberOfShots);

    void drawPlayer(sf::RenderWindow &window);

    void lightUpFloorBall(std::vector<floorBall> &ballList);

    void shoot(int x, int y, std::vector<Particle> &particleArray);
    void doubleShoot(int x, int y, std::vector<Particle> &particleArray);

    void updateCenterCoordinates();

    void movePlayer(int FPS, sf::RenderWindow &window, std::vector<floorBall> &ballList);

    void drawPlayerShoots(sf::RenderWindow &window);

    void movePlayerShoots();
    void fillShotArray();
};