#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <memory>

#include "enemies.hpp"
#include "projectiles.hpp"
#include "exceptii.hpp"

#ifndef TOWERS_HPP
#define TOWERS_HPP

class Tower
{
protected:
    bool isBeingSet, isBeingPlaced, isBeingRotated, isFirst = 1;
    sf::CircleShape shape;
    sf::RectangleShape attackRangeShape;
    sf::FloatRect boundingBox, attackRange;
    sf::Time attackSpeed;
    sf::Clock attackTimer;
    sf::Vector2f position;

public:
    explicit Tower(sf::Vector2f pos);

    virtual ~Tower();

    void drawTower(sf::RenderWindow &window);

    void drawAttackRange(sf::RenderWindow &window);

    void rotateTower(float ang);

    void hasBeenPlaced();

    void hasBeenRotated();

    bool getPlacedStatus();

    bool getRotatedStatus();

    float findAngle (sf::Vector2f pos);

    void moveTower(sf::Vector2f newPosition);

    bool checkMousePosition (sf::Vector2f mousePosition);

    bool checkAttackRange (sf::Vector2f position);

    void hasBeenSet();

    bool isSet();

    sf::Vector2f getPosition();

    float getRotation();

    bool canShoot();

    virtual void shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f enemyPos, float ang, int index);

    void clockReset();

    void update(std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, std::shared_ptr<Tower> &tow, sf::RenderWindow &window);

    friend std::ostream &operator<<( std::ostream &output, const Tower &tow );

    std::shared_ptr<Tower> clone() const;

    Tower (const Tower &tow);
};

class Tower_01 : public Tower
{
public:
    explicit Tower_01(sf::Vector2f pos);

    ~Tower_01();

    void shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f enemyPos, float ang, int index) override;

    std::shared_ptr<Tower_01> clone() const;
};

class Tower_02 : public Tower
{
public:
    explicit Tower_02(sf::Vector2f pos);

    ~Tower_02();

    void shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f enemyPos, float ang, int index) override;

    std::shared_ptr<Tower_02> clone() const;
};

#endif
