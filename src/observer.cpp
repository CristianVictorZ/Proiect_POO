#include "../headers/observer.hpp"

///Constructor
EnemyObserver::EnemyObserver(std::shared_ptr<Tower> &tow):tower(tow){}

///Seteaza pozitia si indicele unui inamic pentru un turn.
void EnemyObserver::update(sf::Vector2f pos, int targetIndex)
{
    tower->setTargetPosition(pos);
    tower->setTargetIndex(targetIndex);
}

///Intoarce un shared pointer pentru EnemyObserver.
std::shared_ptr<EnemyObserver> EnemyObserver::clone() const
{
    return std::make_shared<EnemyObserver>(*this);
}

///Constructor Subject.
Subject::Subject(){}

///Destructor Subject.
Subject::~Subject(){}

///Incarca un observer in vectorul de observeri.
void Subject::registerObserver(std::shared_ptr<Observer> obs)
{
    observers.emplace_back(obs);
}

///Actualizeaza observerii.
void Subject::notifyObservers()
{
    for(std::shared_ptr<Observer> obs : observers)
        obs->update(position, index);
}

///Seteaza pozitia si indicele unui inamic pentru un observer.
void Subject::setPosition(sf::Vector2f pos, int targetIndex)
{
    position = pos;
    index = targetIndex;
    notifyObservers();
}
