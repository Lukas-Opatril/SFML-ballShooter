#include "main.h"

const int FPS = 60;
const unsigned short int MINIMUM_REGULAR_ENEMY_SIZE_AFTER_HIT = 10;
const unsigned short int ON_HIT_PARTICLE_AMOUNT = 30;
const unsigned short int NUMBER_OF_SHOTS = 6;
const unsigned short int REGULAR_ENEMY_DEFAULT_SPEED = 80;
const float REGULAR_ENEMY_SPAWN_RATE = 2;     // In seconds
const float ENEMY_BOSS_SPAWN_RATE = 0.4 * 60; // In seconds
const float ENEMY_BOSS_SHOOT_RATE = 1;        // One shot per second
const unsigned short int FLOOR_BALL_SIZE = 20;
const float SHOT_RELOUAD_COOLDAWN = 6; // In seconds

using sf::Color;
using std::vector;

std::array<double, 2> generateEnemyXYCoordinates(double &radius, sf::RenderWindow &window)
{
    double x, y;
    std::array<double, 2> returnArray = {};
    if ((double)rand() / RAND_MAX > 0.5)
    {
        if ((double)rand() / RAND_MAX > 0.5)
        {

            x = rand() % window.getSize().x;
            y = -radius;
        }
        else
        {
            x = rand() % window.getSize().x;
            y = window.getSize().y + radius;
        }
    }
    else
    {
        if ((double)rand() / RAND_MAX > 0.5)
        {
            x = -radius;
            y = rand() % window.getSize().y;
        }

        else
        {
            x = window.getSize().x + radius;
            y = rand() % window.getSize().y;
        }
    }
    returnArray[0] = x;
    returnArray[1] = y;
    return returnArray;
}

void spawnRegularEnemy(
    unsigned int counter, std::vector<Enemy> &enemyVector, sf::RenderWindow &window, Color arrayColor)
{
    if (counter % (int)(FPS * REGULAR_ENEMY_SPAWN_RATE) == 0)
    {
        double radius, x, y;
        double size = std::rand() % 50 + 50;
        radius = size / 2;
        std::array<double, 2> xyArray = generateEnemyXYCoordinates(radius, window);
        x = xyArray[0];
        y = xyArray[1];

        enemyVector.emplace_back(Enemy(x, y, size, arrayColor));
    }
}
void spawnBossEnemy(
    unsigned int counter, std::vector<enemyBoss> &enemyBossVector, sf::RenderWindow &window, Color arrayColor, int &ID)
{
    if (counter % (int)(FPS * ENEMY_BOSS_SPAWN_RATE) == 0)
    {
        double radius, x, y;
        double size = std::rand() % 80 + 80;
        radius = size / 2;
        std::array<double, 2> xyArray = generateEnemyXYCoordinates(radius, window);
        x = xyArray[0];
        y = xyArray[1];

        enemyBossVector.emplace_back(enemyBoss(x, y, size, arrayColor, ID));
        ID++;
    }
}

void resetGame(

    vector<Particle> &playerParticleVector,
    vector<Particle> &enemyBossParticleVector,
    vector<dmgParticle> &dmgParticleVector,
    vector<Enemy> &enemyVector,
    vector<enemyBoss> &enemyBossVector,
    Player &player,
    sf::RenderWindow &window,
    float &timeElapsed,
    unsigned int &shotReloudCounter,
    unsigned int &frameCounter,
    bool &isMovingUp,
    bool &isMovingDown,
    bool &isMovingLeft,
    bool &isMovingRight,
    sf::Clock clock,
    vector<floorBall> &floorBallVector

)
{
    playerParticleVector = {};
    enemyBossParticleVector = {};
    dmgParticleVector = {};
    enemyVector = {};
    enemyBossVector = {};
    floorBallVector = {};
    floorBall::fillList(window, FLOOR_BALL_SIZE, floorBallVector);
    player.x = (double)window.getSize().x / 2 - Player::radius;
    player.y = (double)window.getSize().y / 2 - Player::radius;
    player.moveX = 0;
    player.moveY = 0;
    player.circleArray = {};
    player.fillShotArray();
    timeElapsed = 0.0f;
    shotReloudCounter = 0;
    frameCounter = 0;
    isMovingUp = false;
    isMovingDown = false;
    isMovingLeft = false;
    isMovingRight = false;
    clock.restart();
}

int Player::size = 100;
int Player::radius = Player::size / 2;
int Player::DMG = Player::radius / 2;
int Enemy::defaultSpeed = REGULAR_ENEMY_DEFAULT_SPEED;

void refilShots(unsigned int &shotReloudCounter, Player &player)
{

    player.fillShotArray();
    shotReloudCounter = 0;
}

int main()
{

    std::vector<floorBall> floorBallVector = {};
    std::vector<Particle> playerParticleVector = {};
    std::vector<Particle> enemyBossParticleVector = {};
    std::vector<dmgParticle> dmgParticleVector = {};
    std::vector<Enemy> enemyVector = {};
    std::vector<enemyBoss> enemyBossVector = {};

    std::array<Color, 7> colorArray{Color::Blue, Color::Cyan, Color::Green, Color::Magenta, Color::Red, Color::Yellow, Color::White};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::uniform_int_distribution<int> rngColor(0, colorArray.size() - 1);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Lukas Opatril - zaverecna prace", sf::Style::Fullscreen);
    window.setFramerateLimit(FPS);
    Player player((double)window.getSize().x / 2, (double)window.getSize().y / 2, Color::Blue, NUMBER_OF_SHOTS);

    sf::Clock clock;
    float timeElapsed = 0.0f;
    unsigned int shotReloudCounter = 0;
    unsigned int frameCounter = 0;
    int minutes, seconds;
    int ID_counter = 0;

    sf::Font font;

    if (!font.loadFromFile("./src/lib/fonts/arial.ttf"))
    {
        return -1;
    }
    sf::Text timerText("00:00", font, 100);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition((window.getSize().x / 2 - timerText.getLocalBounds().width / 2), 10);

    sf::Text gameNameText("Ball Shooter", font, 100);
    gameNameText.setPosition((window.getSize().x / 2 - gameNameText.getGlobalBounds().width / 2), 100);

    sf::Text gameStartText("Zahajit hru !", font, 85);
    gameStartText.setPosition((window.getSize().x / 2 - gameStartText.getGlobalBounds().width / 2), 250);

    sf::Text gameOverText("GAME OVER", font, 100);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition((window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2), 100);

    sf::SoundBuffer buffer, buffer2, buffer3;
    if (!buffer.loadFromFile("./sounds/shot.wav") || !buffer2.loadFromFile("./sounds/enemyBoss.wav") || !buffer3.loadFromFile("./sounds/pop.wav"))
    {
        return -1;
    }
    sf::Sound playerShoot;
    playerShoot.setBuffer(buffer);

    sf::Sound enemyBossShot;
    enemyBossShot.setBuffer(buffer2);

    sf::Sound enemyPop;
    enemyPop.setBuffer(buffer3);

    floorBall::fillList(window, FLOOR_BALL_SIZE, floorBallVector);

    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

    bool startGame = false;
    bool isGameOver = false;
    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && startGame)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:

                    isMovingUp = true;
                    break;
                case sf::Keyboard::D:

                    isMovingRight = true;
                    break;
                case sf::Keyboard::S:

                    isMovingDown = true;
                    break;
                case sf::Keyboard::A:

                    isMovingLeft = true;
                    break;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && startGame)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                player.shoot(mousePos.x, mousePos.y, playerParticleVector);
                if (player.circleArray.size() > 0)
                {
                    playerShoot.play();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && startGame)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                player.doubleShoot(mousePos.x, mousePos.y, playerParticleVector);
                if (player.circleArray.size() > 1)
                {
                    playerShoot.play();
                }
            }
            if (event.type == sf::Event::KeyReleased && startGame)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    isMovingUp = false;
                    break;
                case sf::Keyboard::D:
                    isMovingRight = false;
                    break;
                case sf::Keyboard::S:
                    isMovingDown = false;
                    break;
                case sf::Keyboard::A:
                    isMovingLeft = false;
                    break;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !startGame)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::FloatRect bounds = gameStartText.getGlobalBounds();
                if (bounds.contains((float)mousePos.x, (float)mousePos.y))
                {

                    startGame = true;
                    if (isGameOver)
                    {
                        isGameOver = false;
                        resetGame(
                            playerParticleVector,
                            enemyBossParticleVector,
                            dmgParticleVector,
                            enemyVector,
                            enemyBossVector,
                            player,
                            window,
                            timeElapsed,
                            shotReloudCounter,
                            frameCounter,
                            isMovingUp,
                            isMovingDown,
                            isMovingLeft,
                            isMovingRight,
                            clock,
                            floorBallVector);
                    }
                }
            }
            if (sf::Event::MouseMoved)
            {
                if (gameStartText.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
                {

                    gameStartText.setStyle(sf::Text::Underlined); // Nastav styl podtržení textu
                }
                else
                {
                    gameStartText.setStyle(sf::Text::Regular); // Nastav styl podtržení textu
                }
            }
        }

        if (isMovingUp)
            player.moveY -= 35;
        if (isMovingDown)
            player.moveY += 35;
        if (isMovingRight)
            player.moveX += 35;
        if (isMovingLeft)
            player.moveX -= 35;

        window.clear();

        if (startGame && !isGameOver)
        {
            if (timeElapsed == 0)
            {
                clock.restart();
            }
            float deltaTime = clock.restart().asSeconds();

            /*
            Metoda clock.restart() vrací aktuální čas a zároveň resetuje hodnotu časovače.
             To znamená, že hodnota vrácená touto metodou se vztahuje k času,
              kdy byl časovač naposledy resetován.
            */

            timeElapsed += deltaTime;
            std::ostringstream ss;
            minutes = (int)(timeElapsed / 60);
            seconds = (int)(timeElapsed - minutes * 60);

            ss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

            timerText.setString(ss.str());

            /*
     Tento kód slouží k formátování textového řetězce, který reprezentuje aktuální časovač.

    Nejprve se vytvoří objekt std::ostringstream jménem ss.
    Do streamu můžeme zapisovat
     různé datové typy a poté je převést na textový řetězec.

    Následně se pomocí operátoru << do streamu ss postupně zapisují
     hodnoty proměnných minutes a seconds. Pomocí funkce std::setw()
     se nastavuje minimální počet znaků, které mají být vyhrazeny pro zápis dané proměnné.

    Takže zde minutes bude mít minimálně dva znaky a seconds minimálně dva znaky.
     Pokud by byla hodnota menší, stream bude doplněn mezerami.

    Dále se pomocí funkce std::setfill() nastavuje,
    jakým znakem se má stream doplnit, pokud je to potřeba. Zde se nastavuje znak '0'.
    Nakonec se pomocí funkce ss.str() převede obsah streamu na textový řetězec
    a ten se nastaví jako obsah textového pole timerText pomocí funkce setString().
            */

            if (timeElapsed >= 3600.0f)
            {
                timeElapsed = 0.0f;
            }

            /*
            Tímto kódem se v případě, že časovač dosáhl hodnoty 3600 sekund (tj. jedné hodiny),
             nastaví na hodnotu 0, čímž se efektivně resetuje.
            */

            for (floorBall &ball : floorBallVector)
            {
                ball.drawBall(window);
            }
            for (Particle &playerParticle : playerParticleVector)
            {
                playerParticle.moveParticle(FPS);
                playerParticle.drawParticle(window);
            }
            for (Enemy &enemy : enemyVector)
            {
                int diffX = player.centerX - enemy.centerX;
                int diffY = player.centerY - enemy.centerY;
                double distance = std::sqrt(diffX * diffX + diffY * diffY);
                double angle = atan2(diffY, diffX);
                enemy.moveEnemy(std::sin(angle), std::cos(angle));
                enemy.drawEnemy(window);
            }
            for (Particle &enemyBossParticle : enemyBossParticleVector)
            {
                enemyBossParticle.moveParticle(FPS);
                enemyBossParticle.drawParticle(window);
            }
            for (enemyBoss &enemyBoss : enemyBossVector)
            {
                int diffX = player.centerX - enemyBoss.centerX;
                int diffY = player.centerY - enemyBoss.centerY;
                double distance = std::sqrt(diffX * diffX + diffY * diffY);
                double angle = atan2(diffY, diffX);
                enemyBoss.moveEnemy(std::sin(angle), std::cos(angle));
                enemyBoss.drawEnemy(window);
                if (frameCounter % (int)(FPS * ENEMY_BOSS_SHOOT_RATE) == 0)
                    if (enemyBoss.centerX > 0 && enemyBoss.centerX < window.getSize().x && enemyBoss.centerY > 0 && enemyBoss.centerY < window.getSize().y)
                    {
                        enemyBoss.shoot(player.centerX, player.centerY, enemyBossParticleVector);
                        enemyBossShot.play();
                    }
            }
            for (auto it = dmgParticleVector.begin(); it != dmgParticleVector.end();)
            {

                /*
                 Použití auto umožňuje kompilátoru odvodit typ iterátoru na základě typu dmgParticleVector.
                Místo toho, aby použití auto, můžeme určit typ iterátoru explicitně pomocí std::vector<dmgParticle>::iterator. Například:

                for (std::vector<dmgParticle>::iterator it = dmgParticleVector.begin(); it != dmgParticleVector.end();)
                {
                    // ...
                }

                */
                dmgParticle &dmgParticle = *it;
                dmgParticle.drawParticle(window);
                dmgParticle.moveParticle(FPS);
                dmgParticle.shrinkParticle();

                if (dmgParticle.radius - dmgParticle.RADIUS_DECREASE <= 1)
                    it = dmgParticleVector.erase(it);
                /*
                Metoda "erase" vrací iterátor ukazující na nové umístění prvku, který následoval za posledním prvkem vymazaným voláním funkce.
                */

                else
                    ++it;
            }

            /*
            Player particle -> regual enemy body
            */

            for (int i = 0; i < playerParticleVector.size(); i++)
            {
                for (int j = 0; j < enemyVector.size(); j++)
                {
                    double diffX = playerParticleVector[i].centerX - enemyVector[j].centerX;
                    double diffY = playerParticleVector[i].centerY - enemyVector[j].centerY;
                    double distance = std::sqrt(diffX * diffX + diffY * diffY);
                    if (distance - playerParticleVector[i].radius - enemyVector[j].radius <= 1)
                    {
                        for (int k = 0; k < ON_HIT_PARTICLE_AMOUNT; k++)
                        {
                            Color enemyColor = enemyVector[j].color;
                            Color clr = Color(enemyColor.r, enemyColor.g, enemyColor.b, 128);
                            double directionX = dist(gen) / 3;
                            double directionY = dist(gen) / 3;
                            dmgParticleVector.emplace_back(dmgParticle(
                                enemyVector[j].centerX, enemyVector[j].centerY,
                                (int)enemyVector[j].radius / 2, clr, directionX, directionY));
                        }
                        playerParticleVector.erase(playerParticleVector.begin() + i);
                        if (enemyVector[j].radius - Player::DMG < MINIMUM_REGULAR_ENEMY_SIZE_AFTER_HIT)
                        {
                            for (floorBall &ball : floorBallVector)
                            {
                                ball.changeColor(enemyVector[j].color, 50);
                            }
                            enemyVector.erase(enemyVector.begin() + j);
                        }
                        else
                        {
                            enemyVector[j].radius -= Player::DMG;
                            enemyVector[j].setSpeed();
                        }
                        i = i - 1; // Decrement the index because the vector size has decreased
                        enemyPop.play();

                        break; // Only one particle can hit one enemy, so move on to the next particle
                    }
                }
            }

            /*
            Player particle --> enemyBoss particle

            */
            for (int i = 0; i < playerParticleVector.size(); i++)
            {
                for (int j = 0; j < enemyBossParticleVector.size(); j++)
                {
                    double diffX = playerParticleVector[i].centerX - enemyBossParticleVector[j].centerX;
                    double diffY = playerParticleVector[i].centerY - enemyBossParticleVector[j].centerY;
                    double distance = std::sqrt(diffX * diffX + diffY * diffY);
                    if (distance - playerParticleVector[i].radius - enemyBossParticleVector[j].radius <= 1)
                    {
                        for (int k = 0; k < ON_HIT_PARTICLE_AMOUNT; k++)
                        {
                            Color enemyBossParticleColor = enemyBossParticleVector[j].shotColor;
                            Color clr = Color(enemyBossParticleColor.r, enemyBossParticleColor.g, enemyBossParticleColor.b, 128);
                            Color clr2 = Color(player.color.r, player.color.g, player.color.b, 128);
                            double directionX = dist(gen) / 3;
                            double directionY = dist(gen) / 3;
                            double directionX2 = dist(gen) / 3;
                            double directionY2 = dist(gen) / 3;

                            dmgParticleVector.emplace_back(dmgParticle(
                                playerParticleVector[i].centerX, playerParticleVector[i].centerY,
                                (int)playerParticleVector[i].radius, clr2, directionX2, directionY2));

                            dmgParticleVector.emplace_back(dmgParticle(
                                enemyBossParticleVector[j].centerX, enemyBossParticleVector[j].centerY,
                                (int)enemyBossParticleVector[j].radius / 2, clr, directionX, directionY));
                        }
                        playerParticleVector.erase(playerParticleVector.begin() + i);
                        enemyBossParticleVector.erase(enemyBossParticleVector.begin() + j);

                        i = i - 1; // Decrement the index because the vector size has decreased
                        enemyPop.play();

                        break; // Only one particle can hit one enemy, so move on to the next particle
                    }
                }
            }

            // Player particle -> boss body
            for (int i = 0; i < playerParticleVector.size(); i++)
            {
                for (int j = 0; j < enemyBossVector.size(); j++)
                {
                    double diffX = playerParticleVector[i].centerX - enemyBossVector[j].centerX;
                    double diffY = playerParticleVector[i].centerY - enemyBossVector[j].centerY;
                    double distance = std::sqrt(diffX * diffX + diffY * diffY);
                    if (distance - playerParticleVector[i].radius - enemyBossVector[j].radius <= 1)
                    {
                        for (int k = 0; k < ON_HIT_PARTICLE_AMOUNT; k++)
                        {
                            Color enemyColor = enemyBossVector[j].color;
                            Color clr = Color(enemyColor.r, enemyColor.g, enemyColor.b, 128);
                            double directionX = dist(gen) / 3;
                            double directionY = dist(gen) / 3;
                            dmgParticleVector.emplace_back(dmgParticle(
                                enemyBossVector[j].centerX, enemyBossVector[j].centerY,
                                (int)enemyBossVector[j].radius / 2, clr, directionX, directionY));
                        }
                        playerParticleVector.erase(playerParticleVector.begin() + i);
                        if (enemyBossVector[j].radius - Player::DMG < MINIMUM_REGULAR_ENEMY_SIZE_AFTER_HIT)
                        {
                            for (floorBall &ball : floorBallVector)
                            {
                                ball.changeColor(enemyBossVector[j].color, 50);
                            }
                            enemyBossVector.erase(enemyBossVector.begin() + j);
                        }
                        else
                        {
                            enemyBossVector[j].radius -= Player::DMG;
                            enemyBossVector[j].setSpeed();
                        }
                        i = i - 1; // Decrement the index because the vector size has decreased
                        enemyPop.play();

                        break; // Only one particle can hit one enemy, so move on to the next particle
                    }
                }
            }

            // Enemy boss particle --> regular enemy body
            for (int i = 0; i < enemyBossParticleVector.size(); i++)
            {
                for (int j = 0; j < enemyVector.size(); j++)
                {
                    double diffX = enemyBossParticleVector[i].centerX - enemyVector[j].centerX;
                    double diffY = enemyBossParticleVector[i].centerY - enemyVector[j].centerY;
                    double distance = std::sqrt(diffX * diffX + diffY * diffY);
                    if (distance - enemyBossParticleVector[i].radius - enemyVector[j].radius <= 1)
                    {
                        for (int k = 0; k < ON_HIT_PARTICLE_AMOUNT; k++)
                        {
                            Color enemyColor = enemyVector[j].color;
                            Color clr = Color(enemyColor.r, enemyColor.g, enemyColor.b, 128);
                            double directionX = dist(gen) / 3;
                            double directionY = dist(gen) / 3;
                            dmgParticleVector.emplace_back(dmgParticle(
                                enemyVector[j].centerX, enemyVector[j].centerY,
                                (int)enemyVector[j].radius / 2, clr, directionX, directionY));
                        }
                        enemyBossParticleVector.erase(enemyBossParticleVector.begin() + i);
                        if (enemyVector[j].radius - enemyBossParticleVector[i].radius < MINIMUM_REGULAR_ENEMY_SIZE_AFTER_HIT)
                        {

                            enemyVector.erase(enemyVector.begin() + j);
                        }
                        else
                        {
                            enemyVector[j].radius -= enemyBossParticleVector[i].radius;
                            enemyVector[j].setSpeed();
                        }
                        i = i - 1; // Decrement the index because the vector size has decreased
                        enemyPop.play();

                        break; // Only one particle can hit one enemy, so move on to the next particle
                    }
                }
            }

            // Enemy boss particle --> enemy boss body
            for (int i = 0; i < enemyBossParticleVector.size(); i++)
            {
                for (int j = 0; j < enemyBossVector.size(); j++)
                {
                    if (enemyBossParticleVector[i].ID == enemyBossVector[j].ID)
                    {
                        continue;
                    }
                    double diffX = enemyBossParticleVector[i].centerX - enemyBossVector[j].centerX;
                    double diffY = enemyBossParticleVector[i].centerY - enemyBossVector[j].centerY;
                    double distance = std::sqrt(diffX * diffX + diffY * diffY);
                    if (distance - enemyBossParticleVector[i].radius - enemyBossVector[j].radius <= 1)
                    {
                        for (int k = 0; k < ON_HIT_PARTICLE_AMOUNT; k++)
                        {
                            Color enemyColor = enemyBossVector[j].color;
                            Color clr = Color(enemyColor.r, enemyColor.g, enemyColor.b, 128);
                            double directionX = dist(gen) / 3;
                            double directionY = dist(gen) / 3;
                            dmgParticleVector.emplace_back(dmgParticle(
                                enemyBossVector[j].centerX, enemyBossVector[j].centerY,
                                (int)enemyBossVector[j].radius / 2, clr, directionX, directionY));
                        }
                        enemyBossParticleVector.erase(enemyBossParticleVector.begin() + i);
                        if (enemyBossVector[j].radius - enemyBossParticleVector[i].radius < MINIMUM_REGULAR_ENEMY_SIZE_AFTER_HIT)
                        {

                            enemyBossVector.erase(enemyBossVector.begin() + j);
                        }
                        else
                        {
                            enemyBossVector[j].radius -= enemyBossParticleVector[i].radius;
                            enemyBossVector[j].setSpeed();
                        }
                        i = i - 1; // Decrement the index because the vector size has decreased
                        enemyPop.play();

                        break; // Only one particle can hit one enemy, so move on to the next particle
                    }
                }
            }

            /*
                Enemy Boss particle -> player body
            */

            for (int j = 0; j < enemyBossParticleVector.size(); j++)
            {
                double diffX = player.centerX - enemyBossParticleVector[j].centerX;
                double diffY = player.centerY - enemyBossParticleVector[j].centerY;
                double distance = std::sqrt(diffX * diffX + diffY * diffY);
                if (distance - Player::radius - enemyBossParticleVector[j].radius <= 1)
                {

                    enemyPop.play();
                    isGameOver = true;
                    startGame = false;
                }
            }

            /*
            Player body -> regualar enemy body

            */
            for (int i = 0; i < enemyVector.size(); i++)
            {
                double diffX = player.centerX - enemyVector[i].centerX;
                double diffY = player.centerY - enemyVector[i].centerY;
                double distance = std::sqrt(diffX * diffX + diffY * diffY);
                if (distance - player.radius - enemyVector[i].radius <= 1)
                {

                    enemyPop.play();
                    isGameOver = true;
                    startGame = false;
                }
            }

            /*
            Player body -> enemyBoss body

            */

            for (int i = 0; i < enemyBossVector.size(); i++)
            {
                double diffX = player.centerX - enemyBossVector[i].centerX;
                double diffY = player.centerY - enemyBossVector[i].centerY;
                double distance = std::sqrt(diffX * diffX + diffY * diffY);
                if (distance - Player::radius - enemyBossVector[i].radius <= 1)
                {

                    enemyPop.play();
                    isGameOver = true;
                    startGame = false;
                }
            }

            player.movePlayer(FPS, window, floorBallVector);
            player.drawPlayer(window);
            player.drawPlayerShoots(window);
            player.movePlayerShoots();
            frameCounter++;
            if (player.circleArray.size() == 0)
            {
                shotReloudCounter++;
                if (shotReloudCounter % (int)(FPS * SHOT_RELOUAD_COOLDAWN) == 0)
                {
                    refilShots(shotReloudCounter, player);
                }
            }
            spawnRegularEnemy(frameCounter, enemyVector, window, colorArray[rngColor(gen)]);
            spawnBossEnemy(frameCounter, enemyBossVector, window, colorArray[rngColor(gen)], ID_counter);
            window.draw(timerText);
        }
        else if (!startGame && !isGameOver)
        {
            window.draw(gameNameText);
            window.draw(gameStartText);
        }
        else if (!startGame && isGameOver)
        {
            window.draw(timerText);
            window.draw(gameStartText);
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}