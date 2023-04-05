#include "floorBall.h"
#include <SFML/Graphics.hpp>



    floorBall::floorBall(int x, int y, int radius, sf::Color color)
    {
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->color = color;
        this->color.a = 128;
    }

    void floorBall::drawBall(sf::RenderWindow &window)
    {
        sf::CircleShape circle(radius);
        circle.setPosition(x, y);
        circle.setFillColor(color);
        window.draw(circle);
    }

    void floorBall::changeColor(sf::Color newColor, sf::Uint8 alpha)
    {
        this->color = sf::Color(newColor.r, newColor.g, newColor.b, alpha);
    }
    void floorBall::fillList(sf::RenderWindow &window, int radius, std::vector<floorBall> &floorBallList)
    {
        for (int x = 0; x < (int)window.getSize().x; x += radius * 3.5)
        {
            for (int y = 0; y < (int)window.getSize().y; y += radius * 3.5)
            {
                floorBallList.emplace_back(floorBall(x, y, radius, sf::Color::Blue));
            }
        }
    }

