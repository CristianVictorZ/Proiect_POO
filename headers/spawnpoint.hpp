#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <vector>

#include "enemies.hpp"

#ifndef SPAWNPOINT_HPP
#define SPAWNPOINT_HPP

class SpawnPoint
{
    sf::Time spawnTime;
    sf::Vector2f position;
    std::vector<sf::Vector2f> checkpoints;
    int enemyType, enemyNumber, generatedEnemies;
    sf::Clock spawnTimer;

public:

    SpawnPoint(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints, sf::Time _spawnTime, int _enemyNumber, int _enemyType);

    ~SpawnPoint();

    sf::Vector2f getSpawnPointPosition();

    std::vector<sf::Vector2f> getCheckpoints();

    int getEnemyNumber();

    int getEnemyType();

    bool canGenerateEnemy();
};

#endif
