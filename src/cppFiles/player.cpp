#define _USE_MATH_DEFINES

#include "player.h"
#include "particle.h"
#include <cmath>
#include <iostream>

Player::Player(double x, double y, sf::Color color, const unsigned short int numberOfShots)
{
    this->x = x - Player::radius;
    this->y = y - Player::radius;
    this->color = color;
    centerX = this->x + Player::radius;
    centerY = this->y + Player::radius;
    this->numberOfShots = numberOfShots;
    this->shotRadius = Player::radius / 2;
    this->angleStep = 0;
    fillShotArray();
}

void Player::drawPlayer(sf::RenderWindow &window)
{
    sf::CircleShape circle(Player::radius);
    circle.setPosition(x, y);
    circle.setFillColor(color);
    window.draw(circle);
}

void Player::lightUpFloorBall(std::vector<floorBall> &ballList)
{
    for (floorBall &ball : ballList)
    {
        int diffX = centerX - ball.x;
        int diffY = centerY - ball.y;
        double distance = std::sqrt((diffX * diffX) + (diffY * diffY));
        if (distance < Player::radius * 3)
        {
            ball.changeColor(ball.color, 255);
        }
        else
        {
            ball.changeColor(ball.color, 50);
        }
    }
}
void Player::shoot(int x, int y, std::vector<Particle> &particleArray)
{
    if (this->circleArray.size() > 0)
    {
        int distanceX = x - centerX;
        int distanceY = y - centerY;
        double angle = atan2(distanceY, distanceX);
        double sin = std::sin(angle) * 1.5;
        double cos = std::cos(angle) * 1.5;
        particleArray.emplace_back(Particle(centerX, centerY, Player::radius, color, cos, sin, NULL));
        this->circleArray.pop_back();
    }
}
void Player::doubleShoot(int x, int y, std::vector<Particle> &particleArray)
{
    if (this->circleArray.size() > 1)
    {
        int distanceX = x - centerX;
        int distanceY = y - centerY;
        double angle = atan2(distanceY, distanceX);
        double sin = std::sin(angle) * 1.5;
        double cos = std::cos(angle) * 1.5;
        particleArray.emplace_back(Particle(centerX, centerY + Player::radius, Player::radius, color, cos, sin, NULL));
        this->circleArray.pop_back();

        int distanceX2 = x - centerX;
        int distanceY2 = y - centerY;
        double angle2 = atan2(distanceY, distanceX);
        double sin2 = std::sin(angle) * 1.5;
        double cos2 = std::cos(angle) * 1.5;
        particleArray.emplace_back(Particle(centerX, centerY, Player::radius, color, cos2, sin2, NULL));
        this->circleArray.pop_back();
    }
}

void Player::updateCenterCoordinates()
{
    centerX = (x + Player::radius);
    centerY = (y + Player::radius);
}

void Player::drawPlayerShoots(sf::RenderWindow &window)
{
    for (int i = 0; i < circleArray.size(); i++)
    {
        window.draw(circleArray[i]);
    }
}
void Player::movePlayerShoots()
{
    for (int i = 0; i < this->numberOfShots; i++)
    {
        // Vypočítání úhlu pro daný kruh
        double angle = i * (360 / this->numberOfShots) + this->angleStep;
        if (angle >= 360)
        {
            angle -= 360;
        }
        // std::cout << angle << std::endl;

        /*
        Tím, že se angleStep po dosažení úhlu 360 stupňů
         zmenší o 360, se zaručí, že úhel se bude držet v rozmezí 0 až 359 stupňů.
          To zajistí, že kruhy zůstanou v oblasti kolem hráče a budou se dál pohybovat
           po směru hodinových ručiček.*/

        // Výpočet nových souřadnic pro kruh
        float shotX = (this->x + shotRadius) + shotRadius * 4 * std::cos(angle * 3.14 / 180);
        float shotY = (this->y + shotRadius) + shotRadius * 4 * std::sin(angle * 3.14 / 180);

        // Nastavení nových souřadnic pro kruh
        circleArray[i].setPosition(shotX, shotY);
    }

    // Inkrementace kroku pro další volání funkce
    this->angleStep += 2;
}

void Player::fillShotArray()
{

    for (int i = 0; i < this->numberOfShots; i++)
    {
        sf::CircleShape shot(shotRadius);
        float angle = i * (360 / this->numberOfShots);                                        // Vypočítání úhlu pro umístění menšího kruhu
        float shotX = (this->x + shotRadius) + shotRadius * 4 * std::cos(angle * 3.14 / 180); // Výpočet x-ové souřadnice
        float shotY = (this->y + shotRadius) + shotRadius * 4 * std::sin(angle * 3.14 / 180); // Výpočet y-ové souřadnice
        shot.setPosition(shotX, shotY);
        shot.setFillColor(this->color);
        circleArray.emplace_back(shot);
    }
}

void Player::movePlayer(int FPS, sf::RenderWindow &window, std::vector<floorBall> &ballList)
{
    if (centerX + Player::radius >= window.getSize().x && moveX > 0)
    {
        moveX = -moveX / 2;
    }
    if (centerX - Player::radius <= 0 && moveX < 0)
    {
        moveX = -moveX / 2;
    }
    if (centerY - Player::radius <= 0 && moveY < 0)
    {
        moveY = -moveY / 2;
    }
    if (centerY + Player::radius >= window.getSize().y && moveY > 0)
    {
        moveY = -moveY / 2;
    }
    x += moveX / FPS;
    y += moveY / FPS;
    if (moveX != 0)
    {
        if (moveX < 0)
        {
            moveX += 20;
        }
        else if (moveX > 0)
        {
            moveX -= 20;
        }
    }
    if (moveY != 0)
    {
        if (moveY < 0)
        {
            moveY += 20;
        }
        else if (y > 0)
        {
            moveY -= 20;
        }
    }
    Player::updateCenterCoordinates();
    Player::lightUpFloorBall(ballList);
};
