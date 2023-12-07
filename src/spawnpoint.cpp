#include "spawnpoint.hpp"

SpawnPoint::SpawnPoint(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints, sf::Time _spawnTime, int _enemyNumber, int _enemyType) :
            spawnTime(_spawnTime), position(_position), checkpoints(_checkpoints), enemyType(_enemyType), enemyNumber(_enemyNumber)
            {generatedEnemies = 0;}

SpawnPoint::~SpawnPoint(){}

sf::Vector2f SpawnPoint::getSpawnPointPosition()
{
    return position;
}

std::vector<sf::Vector2f> SpawnPoint::getCheckpoints()
{
    return checkpoints;
}

int SpawnPoint::getEnemyNumber()
{
    return enemyNumber;
}

int SpawnPoint::getEnemyType()
{
    return enemyType;
}

//    int SpawnPoint::getGeneratedEnemies()
//    {
//        return generatedEnemies;
//    }

bool SpawnPoint::canGenerateEnemy()
{
    sf::Time elapsedTime = spawnTimer.getElapsedTime();
    if(elapsedTime >= spawnTime && generatedEnemies++ < enemyNumber)
    {
        spawnTimer.restart();
        return 1;
    }
    return 0;
}
