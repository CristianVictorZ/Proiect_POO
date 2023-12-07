#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "projectiles.hpp"

#ifndef ENEMIES_HPP
#define ENEMIES_HPP

class Enemy
{
protected:
    int lastCheckpoint;
    float speed, maxHealth, health;
    sf::Vector2f position;
    sf::RectangleShape healthBarEmpty, healthBarFull;
    sf::CircleShape shape, moveShape;
    sf::FloatRect boundingBox, moveBox;
    std::vector<sf::Vector2f> checkpoints;

public:

    Enemy(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints);

    virtual ~Enemy();

    void decreaseContor();

    void drawEnemy(sf::RenderWindow &window);

    sf::Vector2f getPosition();

    void updatePosition();

    bool reachedObjective();

    bool isDead(float damageTaken);

    void enemyRemove(std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, int index);

    std::shared_ptr<Enemy> clone() const;

    Enemy(const Enemy &enm);
};

class Enemy_01 : public Enemy
{
public:

    Enemy_01(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints);

    ~Enemy_01();

    std::shared_ptr<Enemy_01> clone() const;
};

class Enemy_02 : public Enemy
{
public:

    Enemy_02(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints);

    ~Enemy_02();

    std::shared_ptr<Enemy_02> clone() const;
};

#endif
