#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#ifndef LEVEL_HPP
#define LEVEL_HPP

class Level
{
    int lives = 5, enemyNumber = 0, defeatedEnemies = 0;
    const int spawnPointNumber = 2;
    const std::vector<sf::Vector2f> spawnPointPositions = {sf::Vector2f(220.f,260.f),sf::Vector2f(500.f,220.f)};
    const std::vector<sf::Time> spawnPointTimers = {sf::seconds(2.f), sf::seconds(4.f)};
    const std::vector<int> spawnPointEnemyNumber = {10,5};
    const std::vector<sf::Vector2f> objectivePositions = {sf::Vector2f(980.f,340.f)};
    const std::vector<int> spawnPointEnemyTypes = {1,2};
    const std::vector<std::vector<sf::Vector2f>> checkpoints =
    {{sf::Vector2f(220.f,260.f),sf::Vector2f(220.f,620.f),sf::Vector2f(540.f,620.f),sf::Vector2f(540.f,500.f),sf::Vector2f(980.f,500.f),sf::Vector2f(980.f,340.f)},
    {sf::Vector2f(500.f,220.f),sf::Vector2f(500.f,380.f),sf::Vector2f(620.f,380.f),sf::Vector2f(620.f,220.f),sf::Vector2f(980.f,220.f),sf::Vector2f(980.f,340.f)}};

public:

    Level();

    ~Level();

    int getSpawnPointNumber();

    int getEnemyType(int k);

    int getSpawnPointEnemyNumber(int k);

    sf::Vector2f getSpawnPointPositions(int k);

    std::vector<sf::Vector2f> getCheckpoints(int k);

    sf::Time getSpawnPointTimer(int k);

    void setEnemyNumber(int _enemyNumber);

    void deductLives();

    void levelLost (sf::RenderWindow &window);

    void enemyDefeated();

    bool levelWon (sf::RenderWindow &window);

    int getLives();
};

#endif
