#include "../headers/towers.hpp"

///Constructor
Tower::Tower(sf::Vector2f pos) : position(pos)
        {isBeingSet = 0; isBeingPlaced = 1; isBeingRotated = 0; isFirst = 0;
        shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Green); shape.setOrigin(30.f, 30.f); shape.setPosition(position);
        attackRangeShape.setSize(sf::Vector2f(120.f,160.f)); attackRangeShape.setFillColor(sf::Color(255,0,0,128)); attackRangeShape.setOrigin(60.f, 140.f); attackRangeShape.setPosition(position);
        boundingBox = shape.getGlobalBounds(); attackRange = attackRangeShape.getGlobalBounds();
        if(position.x < 0 || position.x > 1200 || position.y < 0 || position.y > 900) throw exceptionTowerPosition();}

///Destructor.
Tower::~Tower(){}

///Deseneaza turnul.
void Tower::drawTower(sf::RenderWindow &window)
{
    window.draw(shape);
}

///Deseneaza zona in care turnul poate ataca.
void Tower::drawAttackRange(sf::RenderWindow &window)
{
    window.draw(attackRangeShape);
}

///Roteste turnul si zona in care acesta poate ataca.
void Tower::rotateTower(float ang)
{
    shape.setRotation(ang);
    if(!isBeingSet)
    {
        attackRangeShape.setRotation(ang);
        attackRange = attackRangeShape.getGlobalBounds();
    }
}

///Confirma ca turnul a fost pus jos, iar acum trebuie aleasa directia sa.
void Tower::hasBeenPlaced()
{
    isBeingPlaced = 0;
    isBeingRotated = 1;
}

///Confirma ca turnul a fost rotit.
void Tower::hasBeenRotated()
{
    isBeingRotated = 0;
}

///Intoarce daca turnul a fost pus jos sau nu.
bool Tower::getPlacedStatus()
{
    return isBeingPlaced;
}

///Intoarce daca turnul inca este rotit sau nu.
bool Tower::getRotatedStatus()
{
    return isBeingRotated;
}

///Calculeaza unghiul dintre turn si un punct
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

///Muta turnul la o noua pozitie.
void Tower::moveTower(sf::Vector2f newPosition)
{
    shape.setPosition(newPosition);
    attackRangeShape.setPosition(newPosition);
    boundingBox = shape.getGlobalBounds();
    position = newPosition;
    attackRange = attackRangeShape.getGlobalBounds();
}

///Verifica daca mouse-ul este pe turn.
bool Tower::checkMousePosition (sf::Vector2f mousePosition)
{
    return boundingBox.contains(mousePosition);
}

///Verifica daca o pozitie se afla in zona de atac a turnului.
bool Tower::checkAttackRange (sf::Vector2f position)
{
    return attackRange.contains(position);
}

///Seteaza pozitia tintei turnului; necesara pentru observer
void Tower::setTargetPosition(sf::Vector2f position)
{
    targetPosition = position;
}

///Seteaza indexul tintei din lista de inamici; necesara pentru observer
void Tower::setTargetIndex(int index)
{
    targetIndex = index;
}

///Confirma ca turnul a fost setat (pus + rotit)
void Tower::hasBeenSet()
{
    isBeingSet = 1;
}

///Verifica daca turnul inca este setat (pus + rotit)
bool Tower::isSet()
{
    return isBeingSet;
}

///Intoarce pozitia turnului.
sf::Vector2f Tower::getPosition()
{
    return position;
}

///Intoarce rotatia turnului.
float Tower::getRotation()
{
    return shape.getRotation();
}

///Verifica daca turnul poate trage.
bool Tower::canShoot()
{
    sf::Time elapsedTime = attackTimer.getElapsedTime();
    return (elapsedTime >= attackSpeed);
}

///Reseteaza timer-ul pentru atac dupa ce turnul a tras.
void Tower::clockReset()
{
    attackTimer.restart();
}

///Actualizeaza un turn; Varianta fara Observer
//void Tower::update(std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, std::shared_ptr<Tower> &tow, sf::RenderWindow &window)
//{
//    if(this->canShoot())///Verifica daca turnul poate trage
//    {
//        for(int j=0; j<(int) enemies.size(); j++)///Cauta o tinta in lista de inamici
//        {
//            if(this->isSet() && this->checkAttackRange(enemies[j]->getPosition()))///Daca turnul a fost setat si a gasit o tinta in zona sa de atac, trage.
//            {
//                this->clockReset();
//                float ang = this->findAngle(enemies[j]->getPosition());
//                this->rotateTower(ang);
//
//                ///Decide ce fel de proiectil sa traga in functie de tipul de turn.
//                std::shared_ptr<Tower_01> tw1 = std::dynamic_pointer_cast<Tower_01>(tow);
//                if(tw1) tw1->shoot_01(projectiles, enemies[j]->getPosition(), ang, j);
//
//                std::shared_ptr<Tower_02> tw2 = std::dynamic_pointer_cast<Tower_02>(tow);
//                if(tw2) tw2->shoot_02(projectiles, enemies[j]->getPosition(), ang, j);
//
//                break;
//            }
//        }
//    }
//    ///Deseneaza turnul
//    this->drawTower(window);
//    if(this->getPlacedStatus() || this->getRotatedStatus())
//        this->drawAttackRange(window);
//}

//Cu Observer
void Tower::update(std::vector<std::shared_ptr<Projectile>> &projectiles, std::shared_ptr<Tower> &tow, sf::RenderWindow &window)
{
    if(this->canShoot())///Verifica daca turnul poate trage
    {
        if(this->isSet() && this->checkAttackRange(targetPosition))///Daca turnul a fost setat si are o tinta in zona sa de atac, trage.
            {
                this->clockReset();
                float ang = this->findAngle(targetPosition);
                this->rotateTower(ang);

                ///Decide ce fel de proiectil sa traga in functie de tipul de turn.
                std::shared_ptr<Tower_01> tw1 = std::dynamic_pointer_cast<Tower_01>(tow);
                if(tw1) tw1->shoot_01(projectiles, targetPosition, ang, targetIndex);

                std::shared_ptr<Tower_02> tw2 = std::dynamic_pointer_cast<Tower_02>(tow);
                if(tw2) tw2->shoot_02(projectiles, targetPosition, ang, targetIndex);
            }
    }
    ///Deseneaza turnul
    this->drawTower(window);
    if(this->getPlacedStatus() || this->getRotatedStatus())
        this->drawAttackRange(window);
}

///Supraincarcarea operatorului de afisare.
std::ostream &operator<<(std::ostream &output, const Tower &tow )
{
    output<<"Pozitie : x="<<tow.position.x<<", y="<<tow.position.y<<"; Rotatie : "<<tow.shape.getRotation()<<"\n";
    return output;
}

///Intoarce un shared pointer pentru Tower
std::shared_ptr<Tower> Tower::clone() const
{
    return std::make_shared<Tower>(*this);
}

///Constructor de copiere
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

///Constructor pentru Tower_01
Tower_01::Tower_01(sf::Vector2f pos) : Tower(pos)
        {shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Green); shape.setOrigin(30.f, 30.f); shape.setPosition(position); attackSpeed = sf::seconds(0.5);}

///Destructor pentru Tower_01
Tower_01::~Tower_01(){}

///Functia de tras pentru Tower_01
void Tower_01::shoot_01(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f pos, float ang, int index)
{
    try {
        projectiles.emplace_back(Projectile_01(pos,ang,this->position,index).clone());
    }
    catch(std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

///Intoarce un shared pointer pentru Tower_01
std::shared_ptr<Tower_01> Tower_01::clone() const
{
    return std::make_shared<Tower_01>(*this);
}

///Constructor pentru Tower_02
Tower_02::Tower_02(sf::Vector2f pos) : Tower(pos)
        {shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Yellow); shape.setOrigin(30.f, 30.f); shape.setPosition(position); attackSpeed = sf::seconds(0.25);}

///Destructor pentru Tower_02
Tower_02::~Tower_02(){}

///Functia de tras pentru Tower_02
void Tower_02::shoot_02(std::vector<std::shared_ptr<Projectile>> &projectiles, sf::Vector2f pos, float ang, int index)
{
    try {
        projectiles.emplace_back(Projectile_02(pos,ang,this->position,index).clone());
    }
    catch(std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

///Intoarce un shared pointer pentru Tower_02
std::shared_ptr<Tower_02> Tower_02::clone() const
{
    return std::make_shared<Tower_02>(*this);
}
