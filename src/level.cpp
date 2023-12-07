#include "level.hpp"

Level::Level(){}

Level::~Level(){}

int Level::getSpawnPointNumber()
{
    return spawnPointNumber;
}

int Level::getEnemyType(int k)
{
    return spawnPointEnemyTypes[k];
}

int Level::getSpawnPointEnemyNumber(int k)
{
    return spawnPointEnemyNumber[k];
}

sf::Vector2f Level::getSpawnPointPositions(int k)
{
    return spawnPointPositions[k];
}

std::vector<sf::Vector2f> Level::getCheckpoints(int k)
{
    return checkpoints[k];
}

sf::Time Level::getSpawnPointTimer(int k)
{
    return spawnPointTimers[k];
}

void Level::setEnemyNumber(int _enemyNumber)
{
    enemyNumber += _enemyNumber;
}

void Level::deductLives()
{
    lives--;
}

void Level::levelLost (sf::RenderWindow &window)
{
    window.close();
}

void Level::enemyDefeated()
{
    defeatedEnemies++;
}

bool Level::levelWon (sf::RenderWindow &window)
{
    if(enemyNumber == defeatedEnemies)
    {
        window.close();
        return 1;
    }
    return 0;
}

int Level::getLives()
{
    return lives;
}
