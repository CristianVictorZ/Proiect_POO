#include "../headers/towers.hpp"

Tower::Tower(sf::Vector2f pos) : position(pos)
        {isBeingSet = 0; isBeingPlaced = 1; isBeingRotated = 0; isFirst = 0;
        shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Green); shape.setOrigin(30.f, 30.f); shape.setPosition(position);
        attackRangeShape.setSize(sf::Vector2f(120.f,160.f)); attackRangeShape.setFillColor(sf::Color(255,0,0,128)); attackRangeShape.setOrigin(60.f, 140.f); attackRangeShape.setPosition(position);
        boundingBox = shape.getGlobalBounds(); attackRange = attackRangeShape.getGlobalBounds();
        if(position.x < 0 || position.x > 1200 || position.y < 0 || position.y > 900) throw exceptionTowerPosition();}

Tower::~Tower(){}

void Tower::drawTower(sf::RenderWindow &window)
{
    window.draw(shape);
}

void Tower::drawAttackRange(sf::RenderWindow &window)
{
    window.draw(attackRangeShape);
}

void Tower::rotateTower(float ang)
{
    shape.setRotation(ang);
    if(!isBeingSet)
    {
        attackRangeShape.setRotation(ang);
        attackRange = attackRangeShape.getGlobalBounds();
    }
}

void Tower::hasBeenPlaced()
{
    isBeingPlaced = 0;
    isBeingRotated = 1;
}

void Tower::hasBeenRotated()
{
    isBeingRotated = 0;
}

bool Tower::getPlacedStatus()
{
    return isBeingPlaced;
}

bool Tower::getRotatedStatus()
{
    return isBeingRotated;
}

float Tower::findAngle (sf::Vector2f pos)
{
    float ang,co,ca,ip;
    co = pos.x - position.x;
    ca = pos.y - position.y;
    ip = sqrt(co*co + ca*ca);
    if(!ip)
        return 0;
    ang = (180*asin(co/ip)) / M_PI;

    if(ca > 0 && co < 0)
        return ang*(-1) + 180;
    if(ca > 0)
        return 180 - ang;
    if(co < 0)
        return 360 + ang;
    return ang;
}

void Tower::moveTower(sf::Vector2f newPosition)
{
    shape.setPosition(newPosition);
    attackRangeShape.setPosition(newPosition);
    boundingBox = shape.getGlobalBounds();
    position = newPosition;
    attackRange = attackRangeShape.getGlobalBounds();
}

bool Tower::checkMousePosition (sf::Vector2f mousePosition)
{
    return boundingBox.contains(mousePosition);
}

bool Tower::checkAttackRange (sf::Vector2f position)
{
    return attackRange.contains(position);
}

void Tower::hasBeenSet()
{
    isBeingSet = 1;
}

bool Tower::isSet()
{
    return isBeingSet;
}

sf::Vector2f Tower::getPosition()
{
    return position;
}

float Tower::getRotation()
{
    return shape.getRotation();
}

bool Tower::canShoot()
{
    sf::Time elapsedTime = attackTimer.getElapsedTime();
    return (elapsedTime >= attackSpeed);
}

void Tower::shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f pos, float ang, int index)
{
    projectiles.emplace_back(Projectile(pos,ang,this->position,index).clone());
}

void Tower::clockReset()
{
    attackTimer.restart();
}

void Tower::update(std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, std::shared_ptr<Tower> &tow, sf::RenderWindow &window)
{
    if(this->canShoot())
    {
        for(int j=0; j<(int) enemies.size(); j++)
        {
            if(this->isSet() && this->checkAttackRange(enemies[j]->getPosition()))
            {
                this->clockReset();
                float ang = this->findAngle(enemies[j]->getPosition());
                this->rotateTower(ang);

                std::shared_ptr<Tower_01> tw1 = std::dynamic_pointer_cast<Tower_01>(tow);
                if(tw1) tw1->shoot(projectiles, enemies[j]->getPosition(), ang, j);

                std::shared_ptr<Tower_02> tw2 = std::dynamic_pointer_cast<Tower_02>(tow);
                if(tw2) tw2->shoot(projectiles, enemies[j]->getPosition(), ang, j);

                break;
            }
        }
    }
    this->drawTower(window);
    if(this->getPlacedStatus() || this->getRotatedStatus())
        this->drawAttackRange(window);
}

std::ostream &operator<<(std::ostream &output, const Tower &tow )
{
    output<<"Pozitie : x="<<tow.position.x<<", y="<<tow.position.y<<"; Rotatie : "<<tow.shape.getRotation()<<"\n";
    return output;
}

std::shared_ptr<Tower> Tower::clone() const
{
    return std::make_shared<Tower>(*this);
}

Tower::Tower (const Tower &tow)
{
    this->shape.setRadius(tow.shape.getRadius());
    this->shape.setPointCount(tow.shape.getPointCount());
    this->shape.setFillColor(tow.shape.getFillColor());
    this->shape.setOrigin(tow.shape.getOrigin());
    this->position = tow.position;
    this->shape.setPosition(this->position);
    this->boundingBox = this->shape.getGlobalBounds();

    this->attackRangeShape = tow.attackRangeShape;
    this->attackRange = attackRangeShape.getGlobalBounds();
    this->attackSpeed = tow.attackSpeed;

    this->isBeingPlaced = tow.isBeingPlaced;
    this->isBeingRotated = tow.isBeingRotated;
    this->isBeingSet = tow.isBeingSet;
}

Tower_01::Tower_01(sf::Vector2f pos) : Tower(pos)
        {shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Green); shape.setOrigin(30.f, 30.f); shape.setPosition(position); attackSpeed = sf::seconds(0.5);}

Tower_01::~Tower_01(){}

void Tower_01::shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f pos, float ang, int index)
{
    try {
        projectiles.emplace_back(Projectile_01(pos,ang,this->position,index).clone());
    }
    catch(std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

std::shared_ptr<Tower_01> Tower_01::clone() const
{
    return std::make_shared<Tower_01>(*this);
}

Tower_02::Tower_02(sf::Vector2f pos) : Tower(pos)
        {shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Yellow); shape.setOrigin(30.f, 30.f); shape.setPosition(position); attackSpeed = sf::seconds(0.25);}

Tower_02::~Tower_02(){}

void Tower_02::shoot(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f pos, float ang, int index)
{
    try {
        projectiles.emplace_back(Projectile_02(pos,ang,this->position,index).clone());
    }
    catch(std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

std::shared_ptr<Tower_02> Tower_02::clone() const
{
    return std::make_shared<Tower_02>(*this);
}
