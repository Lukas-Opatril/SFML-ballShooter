#pragma once
#include<SFML/Graphics.hpp>
class floorBall
{
public:
    int x, y;
    int radius;
    sf::Color color;
    floorBall(int x, int y, int radius, sf::Color color);

    void drawBall(sf::RenderWindow &window);

    void changeColor(sf::Color newColor, sf::Uint8 alpha);

    static void fillList(sf::RenderWindow &window, int radius, std::vector<floorBall> &floorBallList);
};
