#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "towers.hpp"

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Observer
{
public:
    virtual void update(sf::Vector2f pos, int targetIndex) = 0;
};

class EnemyObserver : public Observer
{
    std::shared_ptr<Tower> tower;
public:

    EnemyObserver(std::shared_ptr<Tower> &tow);

    void update(sf::Vector2f pos, int targetIndex) override;

    std::shared_ptr<EnemyObserver> clone() const;
};

class Subject
{
    sf::Vector2f position;
    int index;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    void registerObserver(std::shared_ptr<Observer> obs);

    void notifyObservers();

    void setPosition(sf::Vector2f pos, int targetIndex);
};

#endif
