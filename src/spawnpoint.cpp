#include "../headers/spawnpoint.hpp"

///Constructor
SpawnPoint::SpawnPoint(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints, sf::Time _spawnTime, int _enemyNumber, int _enemyType) :
            spawnTime(_spawnTime), position(_position), checkpoints(_checkpoints), enemyType(_enemyType), enemyNumber(_enemyNumber)
            {generatedEnemies = 0;}

///Destructor
SpawnPoint::~SpawnPoint(){}

///Intoarce pozitia unui spawnpoint
sf::Vector2f SpawnPoint::getSpawnPointPosition()
{
    return position;
}

///Intoarce vectorul de checkpoint-uri pentru un spawnpoint.
std::vector<sf::Vector2f> SpawnPoint::getCheckpoints()
{
    return checkpoints;
}

///Intoarce numarul de inamici total pe care il genereaza un spawnpoint.
int SpawnPoint::getEnemyNumber()
{
    return enemyNumber;
}

///Intoarce tipul de inamic generat de un spawnpoint.
int SpawnPoint::getEnemyType()
{
    return enemyType;
}

///Verifica daca un spawnpoint poate genera un inamic.
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
