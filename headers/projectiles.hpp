#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <memory>

#include "exceptii.hpp"

#ifndef PROJECTILES_HPP
#define PROJECTILES_HPP

class Projectile
{
protected:
    sf::Vector2f position;
    sf::RectangleShape shape;
    sf::FloatRect boundingBox;
    sf::Vector2f targetPosition, currentPosition;
    bool targetReached = 0;
    int targetIndex;
    float damage, projectileSpeed;

public:

    Projectile(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex);

    virtual ~Projectile();

    void moveProjectile(sf::RenderWindow &window);

    void decreaseTargetIndex();

    bool reachedTarget();

    int getTargetIndex();

    float getDamage();

    std::shared_ptr<Projectile> clone() const;

    Projectile (const Projectile &proj);
};

class Projectile_01 : public Projectile
{
public:
    Projectile_01(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex);

    ~Projectile_01();

    std::shared_ptr<Projectile_01> clone() const;
};

class Projectile_02 : public Projectile
{
public:
    Projectile_02(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex);

    ~Projectile_02();

    std::shared_ptr<Projectile_02> clone() const;
};

#endif
