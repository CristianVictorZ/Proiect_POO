#include "../headers/enemies.hpp"

///Constructor.
Enemy::Enemy(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints) : position(_position), checkpoints(_checkpoints)
                {lastCheckpoint = 0;
                healthBarEmpty.setSize(sf::Vector2f(36.f,3.6)); healthBarEmpty.setOrigin(sf::Vector2f(18.f,1.8)); healthBarEmpty.setFillColor(sf::Color::Black); healthBarEmpty.setPosition(position);
                healthBarFull.setSize(sf::Vector2f(35.f,3.f)); healthBarFull.setOrigin(sf::Vector2f(17.5,1.5)); healthBarFull.setFillColor(sf::Color(200,0,0)); healthBarFull.setPosition(position);
                moveShape.setRadius(2.f); moveShape.setOrigin(3.f, 3.f); moveShape.setPosition(position);
                boundingBox = shape.getGlobalBounds(); moveBox = moveShape.getGlobalBounds();}

///Destructor.
Enemy::~Enemy(){};


///Deseneaza un inamic si bara de viata a sa.
void Enemy::drawEnemy(sf::RenderWindow &window)
{
    window.draw(shape);
    if(health!=maxHealth)
    {
        window.draw(healthBarEmpty);
        window.draw(healthBarFull);
    }
}

///Intoarce pozitia unui inamic.
sf::Vector2f Enemy::getPosition()
{
    return position;
}

///Actualizeaza si intoarce pozitia unui inamic.
sf::Vector2f Enemy::updatePosition()
{
    if(lastCheckpoint < (int) checkpoints.size() - 1)///Verifica daca a ajuns la fianl (ultimul checkpoint).
    {
        if(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x != 0)///Verifica daca se deplaseaza pe orizontala.
        {
            position.x = position.x + (speed * abs(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x)/(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x));

        }else if(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y != 0)///Verifica daca se deplaseaza pe verticala.
        {
            position.y = position.y + (speed * abs(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y)/(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y));
        }

        ///Deplaseaza inamicul.
        shape.setPosition(position);
        moveShape.setPosition(position);
        healthBarEmpty.setPosition(position.x, position.y + 15.f);
        healthBarFull.setPosition(position.x, position.y + 15.f);
        boundingBox = shape.getGlobalBounds();
        moveBox = moveShape.getGlobalBounds();
    }

    ///Daca a ajuns la un checkpoint, se va trece la urmatorul checkpoint din vector.
    if(moveBox.contains(checkpoints[lastCheckpoint+1]))
        lastCheckpoint++;
    return position;
}

///Verifica daca a ajuns la obiectiv.
bool Enemy::reachedObjective()
{
    return (lastCheckpoint+1 == (int) checkpoints.size());
}

///Reduce viata inamicului si verifica daca acesta a fost omorat.
bool Enemy::isDead(float damageTaken)
{
    sf::Vector2f currentHealh = sf::Vector2f((health - damageTaken)*30.f/maxHealth, 2.f);
    healthBarFull.setSize(currentHealh);
    return ((health -= damageTaken) <= 0);
}

///Sterge un inamic din vector.
void Enemy::enemyRemove(std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, int index)
{
    enemies.erase(enemies.begin() + index);
    for(int i=0; i<(int) projectiles.size(); i++)
    {
        if(projectiles[i]->getTargetIndex() > index)
            projectiles[i]->decreaseTargetIndex();
    }
}

///Intoarce un shared pointer pentru Enemy.
std::shared_ptr<Enemy> Enemy::clone() const
{
    return std::make_shared<Enemy>(*this);
}

///Constructor de copiere.
Enemy::Enemy(const Enemy &enm)
{
    this->lastCheckpoint = enm.lastCheckpoint;
    this->speed = enm.speed;
    this->maxHealth = enm.maxHealth;
    this->health = enm.health;
    this->position = enm.position;
    this->healthBarEmpty = enm.healthBarEmpty;
    this->healthBarFull = enm.healthBarFull;
    this->shape = enm.shape;
    this->moveShape = enm.moveShape;
    this->checkpoints = enm.checkpoints;
    this->boundingBox = shape.getGlobalBounds();
    this->moveBox = moveShape.getGlobalBounds();
}

///Destructor pentru EnemyFactory.
EnemyFactory::~EnemyFactory(){}

///Intoarce prin EnemyFactory un Enemy_01.
std::shared_ptr<Enemy> Enemy_01Factory::createEnemy(sf::Vector2f position, const std::vector<sf::Vector2f> &checkpoints)
        {return std::make_shared<Enemy_01>(position, checkpoints);}

///Intoarce prin EnemyFactory un Enemy_02.
std::shared_ptr<Enemy> Enemy_02Factory::createEnemy(sf::Vector2f position, const std::vector<sf::Vector2f> &checkpoints)
        {return std::make_shared<Enemy_02>(position, checkpoints);}

///Constructor pentru Enemy_01.
Enemy_01::Enemy_01(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints) : Enemy(_position, _checkpoints)
        {speed = 3; maxHealth = health = 40; shape.setRadius(10.f); shape.setFillColor(sf::Color::Red); shape.setOrigin(10.f, 10.f); shape.setPosition(position);}

///Destructor pentru Enemy_01.
Enemy_01::~Enemy_01(){}

///Constructor pentru Enemy_02.
Enemy_02::Enemy_02(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints) : Enemy(_position, _checkpoints)
        {speed = 5; maxHealth = health = 20; shape.setRadius(10.f); shape.setFillColor(sf::Color::Blue); shape.setOrigin(10.f, 10.f); shape.setPosition(position);}

///Destructor pentru Enemy_02.
Enemy_02::~Enemy_02(){}
