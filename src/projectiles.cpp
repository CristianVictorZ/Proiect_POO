#include "../headers/projectiles.hpp"

///Constructor.
Projectile::Projectile(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex) : position(pos)
                {shape.setSize(sf::Vector2f(8.f,20.f)); shape.setFillColor(sf::Color(255,255,0)); shape.setOrigin(4.f, 10.f); shape.setPosition(position);
                boundingBox = shape.getGlobalBounds(); targetPosition = tPosition; shape.setRotation(angle); currentPosition = position; targetIndex = _targetIndex;
                /*if(targetIndex < 0) throw exceptionProjectileIndex();*/}

///Destructor.
Projectile::~Projectile(){}

///Deplaseaza un proiectil.
void Projectile::moveProjectile(sf::RenderWindow &window)
{
    float dist = sqrt((currentPosition.x - targetPosition.x) * (currentPosition.x - targetPosition.x) + (currentPosition.y - targetPosition.y) * (currentPosition.y - targetPosition.y));
    dist = dist/projectileSpeed;
    sf::Vector2f newPosition = sf::Vector2f((targetPosition.x - currentPosition.x)/dist + currentPosition.x, (targetPosition.y - currentPosition.y)/dist + currentPosition.y);
    targetReached = (((abs(targetPosition.x - currentPosition.x)/(targetPosition.x - currentPosition.x)) != (abs(targetPosition.x - newPosition.x)/(targetPosition.x - newPosition.x))
                      && (targetPosition.x - currentPosition.x) != (targetPosition.x - newPosition.x)) || ((targetPosition.y - currentPosition.y) != (targetPosition.y - newPosition.y) &&
                      (abs(targetPosition.y - currentPosition.y)/(targetPosition.y - currentPosition.y)) != (abs(targetPosition.y - newPosition.y)/(targetPosition.y - newPosition.y))));
    shape.setPosition(newPosition);
    currentPosition = newPosition;
    boundingBox = shape.getGlobalBounds();
    if(!targetReached)
        window.draw(shape);
}

///Scade indexul inamicului pe care il tinteste un proiectil.
void Projectile::decreaseTargetIndex()
{
    targetIndex--;
}

///Verifica daca proiectilul si-a atins tinta.
bool Projectile::reachedTarget()
{
    return targetReached;
}

///Intoarce indexul inamicului pe care il tinteste un proiectil.
int Projectile::getTargetIndex()
{
    return targetIndex;
}

///Intoarce cat damage da un proiectil.
float Projectile::getDamage()
{
    return damage;
}

///Intoarce un shared pointer pentru Projectile.
std::shared_ptr<Projectile> Projectile::clone() const
{
    return std::make_shared<Projectile>(*this);
}

///Constructor de copiere pentru Projectile.
Projectile::Projectile (const Projectile &proj)
{
    this->position = proj.position;
    this->shape = proj.shape;
    this->boundingBox = shape.getGlobalBounds();
    this->targetPosition = proj.targetPosition;
    this->currentPosition = proj.currentPosition;
    this->targetIndex = proj.targetIndex;
    this->targetReached = proj.targetReached;
    this->damage = proj.damage; this->projectileSpeed = proj.projectileSpeed;
}

///Constructor pentru Projectile_01.
Projectile_01::Projectile_01(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex) : Projectile(tPosition, angle, pos, _targetIndex){damage = 20; projectileSpeed = 32;}

///Destructor pentru Projectile_01.
Projectile_01::~Projectile_01(){}

///Intoarce un shared pointer pentru Projectile_01.
std::shared_ptr<Projectile_01> Projectile_01::clone() const
{
    return std::make_shared<Projectile_01>(*this);
}

///Constructor pentru Projectile_02.
Projectile_02::Projectile_02(sf::Vector2f tPosition, int angle, sf::Vector2f pos, int _targetIndex) : Projectile(tPosition, angle, pos, _targetIndex){damage = 10; projectileSpeed = 32;}

///Destructor pentru Projectile_02.
Projectile_02::~Projectile_02(){}

///Intoarce un shared pointer pentru Projectile_02.
std::shared_ptr<Projectile_02> Projectile_02::clone() const
{
    return std::make_shared<Projectile_02>(*this);
}
