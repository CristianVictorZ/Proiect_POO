#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <math.h>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

class Grid : public sf::Drawable, public sf::Transformable
{
    const float width = 30, height = 20;
    const unsigned int len = 40;
    const sf::Vector2u tileSize= sf::Vector2u (len, len);
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = &m_tileset;
        target.draw(m_vertices, states);
    }

public:

    bool load(const std::string& tileset, const int* tiles)
    {
        if (!m_tileset.loadFromFile(tileset))
            return false;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                int tileNumber = tiles[i + j * ((int) width)];

                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

    sf::Vector2f getSnapPoint (sf::Vector2f pos, const int* tiles)
    {
        if(pos.y > len*height)
            return pos;
        int tileNumber = tiles[(int) (pos.x / len) + ((int) (pos.y / len)) * ((int) width)];
        if(tileNumber != 1)
            return pos;
        sf::Vector2i auxPoint = sf::Vector2i((int) pos.x/len, (int) pos.y/len);
        sf::Vector2f snapPoint = sf::Vector2f((float) (auxPoint.x*len + len/2), (float) (auxPoint.y*len + len/2));
        return snapPoint;
    }
};

class Tower
{
    bool isBeingSet, isBeingPlaced, isBeingRotated;
    sf::CircleShape shape;
    sf::RectangleShape attackRangeShape;
    sf::FloatRect boundingBox, attackRange;
    sf::Time attackSpeed = sf::seconds(0.5);
    sf::Clock attackTimer;

protected:

    sf::Vector2f position;
    float damage = 20, projectileSpeed = 8;

public:
    explicit Tower(sf::Vector2f pos) : position(pos)
            {isBeingSet = 0; isBeingPlaced = 1; isBeingRotated = 0;
            shape.setRadius(30.f); shape.setPointCount(3); shape.setFillColor(sf::Color::Green); shape.setOrigin(30.f, 30.f); shape.setPosition(position);
            attackRangeShape.setSize(sf::Vector2f(120.f,160.f)); attackRangeShape.setFillColor(sf::Color(255,0,0,128)); attackRangeShape.setOrigin(60.f, 140.f); attackRangeShape.setPosition(position);
            boundingBox = shape.getGlobalBounds(); attackRange = attackRangeShape.getGlobalBounds();}

    ~Tower(){}

    void drawTower(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

//    void drawAttackRange(sf::RenderWindow &window)
//    {
//        window.draw(attackRangeShape);
//    }

    void rotateTower(float ang)
    {
        shape.setRotation(ang);
        if(!isBeingSet)
        {
            attackRangeShape.setRotation(ang);
            attackRange = attackRangeShape.getGlobalBounds();
        }
    }

    void hasBeenPlaced()
    {
        isBeingPlaced = 0;
        isBeingRotated = 1;
    }

    void hasBeenRotated()
    {
        isBeingRotated = 0;
    }

    bool getPlacedStatus()
    {
        return isBeingPlaced;
    }

    bool getRotatedStatus()
    {
        return isBeingRotated;
    }

    float findAngle (sf::Vector2f pos)
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

    void moveTower(sf::Vector2f newPosition)
    {
        shape.setPosition(newPosition);
        attackRangeShape.setPosition(newPosition);
        boundingBox = shape.getGlobalBounds();
        position = newPosition;
        attackRange = attackRangeShape.getGlobalBounds();
    }

    bool checkMousePosition (sf::Vector2f mousePosition)
    {
        return boundingBox.contains(mousePosition);
    }

    bool checkAttackRange (sf::Vector2f position)
    {
        return attackRange.contains(position);
    }

    void hasBeenSet()
    {
        isBeingSet = 1;
    }

    bool isSet()
    {
        return isBeingSet;
    }

    sf::Vector2f getPosition()
    {
        return position;
    }

    float getRotation()
    {
        return shape.getRotation();
    }

    bool canShoot()
    {
        sf::Time elapsedTime = attackTimer.getElapsedTime();
        if(elapsedTime >= attackSpeed)
        {
            attackTimer.restart();
            return 1;
        }
        return 0;
    }

//    void clockReset()
//    {
//        attackTimer.restart();
//    }

    friend std::ostream &operator<<( std::ostream &output, const Tower &tow )
    {
        output<<"Pozitie : x="<<tow.position.x<<", y="<<tow.position.y<<"; Rotatie : "<<tow.shape.getRotation()<<"\n";
        return output;
    }

    Tower (const Tower &tow)
    {
        this->shape.setRadius(tow.shape.getRadius());
        this->shape.setPointCount(tow.shape.getPointCount());
        this->shape.setFillColor(tow.shape.getFillColor());
        this->shape.setOrigin(tow.shape.getOrigin());
        this->position = tow.position;
        this->shape.setPosition(this->position);
        this->boundingBox = this->shape.getGlobalBounds();
    }
};



class Projectile : public Tower
{
    sf::RectangleShape shape;
    sf::FloatRect boundingBox;
    sf::Vector2f targetPosition, currentPosition;
    bool targetReached = 0;
    int targetIndex;

public:

    Projectile(sf::Vector2f tPosition, int angle, Tower tow, int _targetIndex) : Tower(tow)
                    {shape.setSize(sf::Vector2f(5.f,10.f)); shape.setFillColor(sf::Color::Yellow); shape.setOrigin(2.5, 5.f); shape.setPosition(position);
                    boundingBox = shape.getGlobalBounds(); targetPosition = tPosition; shape.setRotation(angle); currentPosition = position; targetIndex = _targetIndex;}

    ~Projectile(){}

    void moveProjectile(sf::RenderWindow &window)
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

//    bool hitTarget(sf::FloatRect TargetBoundingBox)
//    {
//        return boundingBox.intersects(TargetBoundingBox);
//    }

    bool reachedTarget()
    {
        return targetReached;
    }

    int getTargetIndex()
    {
        return targetIndex;
    }

    float getDamage()
    {
        return damage;
    }
};

class Enemy
{
    int lastCheckpoint;
    float speed, health;
    sf::Vector2f position;
    sf::CircleShape shape, moveShape;
    sf::FloatRect boundingBox, moveBox;
    std::vector<sf::Vector2f> checkpoints;

public:

    Enemy(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints) : position(_position), checkpoints(_checkpoints)
                    {speed = 1; health = 40;  lastCheckpoint = 0;
                    shape.setRadius(10.f); shape.setFillColor(sf::Color::Red); shape.setOrigin(10.f, 10.f); shape.setPosition(position);
                    moveShape.setRadius(2.f); moveShape.setOrigin(3.f, 3.f); moveShape.setPosition(position);
                    boundingBox = shape.getGlobalBounds(); moveBox = moveShape.getGlobalBounds();}

    ~Enemy(){};

    void drawEnemy(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

    sf::Vector2f getPosition()
    {
        return position;
    }

//    sf::FloatRect getBoundingBox()
//    {
//        return boundingBox;
//    }

    void updatePosition()
    {
        if(lastCheckpoint < (int) checkpoints.size() - 1)
        {
            if(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x != 0)
            {
                position.x = position.x + (speed * abs(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x)/(checkpoints[lastCheckpoint+1].x - checkpoints[lastCheckpoint].x));

            }else if(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y != 0)
            {
                position.y = position.y + (speed * abs(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y)/(checkpoints[lastCheckpoint+1].y - checkpoints[lastCheckpoint].y));
            }
            shape.setPosition(position);
            moveShape.setPosition(position);
            boundingBox = shape.getGlobalBounds();
            moveBox = moveShape.getGlobalBounds();
        }
        if(moveBox.contains(checkpoints[lastCheckpoint+1]))
            lastCheckpoint++;
    }

    bool reachedObjective()
    {
        return (lastCheckpoint+1 == (int) checkpoints.size());
    }

    bool isDead(float damageTaken)
    {
        return ((health -= damageTaken) <= 0);
    }
};

class SpawnPoint
{
    int enemyNumber = 10, generatedEnemies;
    sf::Time spawnTime;
    sf::Vector2f position;
    std::vector<sf::Vector2f> checkpoints;
    sf::Clock spawnTimer;

public:

    SpawnPoint(sf::Vector2f _position, const std::vector<sf::Vector2f> &_checkpoints, sf::Time _spawnTime) :  spawnTime(_spawnTime), position(_position), checkpoints(_checkpoints)
                {generatedEnemies = 0;}

    ~SpawnPoint(){}

    sf::Vector2f getSpawnPointPosition()
    {
        return position;
    }

    std::vector<sf::Vector2f> getCheckpoints()
    {
        return checkpoints;
    }

//    int getEnemyNumber()
//    {
//        return enemyNumber;
//    }

//    int getGeneratedEnemies()
//    {
//        return generatedEnemies;
//    }

    bool canGenerateEnemy()
    {
        sf::Time elapsedTime = spawnTimer.getElapsedTime();
        if(elapsedTime >= spawnTime && generatedEnemies++ < enemyNumber)
        {
            spawnTimer.restart();
            return 1;
        }
        return 0;
    }

};

class Level
{
    int lives = 5;
    const int spawnPointNumber = 2;
    const std::vector<sf::Vector2f> spawnPointPositions = {sf::Vector2f(220.f,260.f),sf::Vector2f(500.f,220.f)};
    const std::vector<sf::Time> spawnPointTimers = {sf::seconds(2.f), sf::seconds(4.f)};
    const std::vector<sf::Vector2f> objectivePositions = {sf::Vector2f(980.f,340.f)};
    const std::vector<std::vector<sf::Vector2f>> checkpoints =
    {{sf::Vector2f(220.f,260.f),sf::Vector2f(220.f,620.f),sf::Vector2f(540.f,620.f),sf::Vector2f(540.f,500.f),sf::Vector2f(980.f,500.f),sf::Vector2f(980.f,340.f)},
    {sf::Vector2f(500.f,220.f),sf::Vector2f(500.f,380.f),sf::Vector2f(620.f,380.f),sf::Vector2f(620.f,220.f),sf::Vector2f(980.f,220.f),sf::Vector2f(980.f,340.f)}};

public:

    Level(){}

    ~Level(){}

    int getSpawnPointNumber()
    {
        return spawnPointNumber;
    }

    sf::Vector2f getSpawnPointPositions(int k)
    {
        return spawnPointPositions[k];
    }

    std::vector<sf::Vector2f> getCheckpoints(int k)
    {
        return checkpoints[k];
    }

    sf::Time getSpawnPointTimer(int k)
    {
        return spawnPointTimers[k];
    }

    void deductLives()
    {
        lives--;
    }

    void levelLost (sf::RenderWindow &window)
    {
        window.close();
    }

    int getLives()
    {
        return lives;
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 900), "SFML works!", sf::Style::Default, settings);

    Level level;
    std::vector<SpawnPoint> spawnPoints;
    for(int i=0; i<level.getSpawnPointNumber(); i++)
        spawnPoints.emplace_back(level.getSpawnPointPositions(i), level.getCheckpoints(i), level.getSpawnPointTimer(i));

    Tower tow1(sf::Vector2f(1000.0f,840.0f));
    std::vector<Tower> tow1Inst;
    tow1Inst.reserve(10);

    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;

    const int levelMap[] =
    {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 3, 0, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    };

    Grid grid;

    if (!grid.load("textures./tileset.png", levelMap))
        return -1;

    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = sf::Vector2f((float) event.mouseButton.x, (float) event.mouseButton.y);
                    sf::Vector2f snapPos = grid.getSnapPoint(mousePos, levelMap);
                    if(tow1Inst[tow1Inst.size()-1].getPlacedStatus() && snapPos != mousePos)
                    {
                        tow1Inst[tow1Inst.size()-1].hasBeenPlaced();
                    }else if(tow1Inst[tow1Inst.size()-1].getRotatedStatus())
                    {
                        tow1Inst[tow1Inst.size()-1].hasBeenRotated();
                        float ang = tow1Inst[tow1Inst.size()-1].findAngle(mousePos);
                        ang = (((int) (ang/45 + 1)) % 8)/2;
                        tow1Inst[tow1Inst.size()-1].rotateTower(ang*90);
                        tow1Inst[tow1Inst.size()-1].hasBeenSet();
                    }else if(tow1.checkMousePosition(mousePos))
                    {
                        tow1Inst.emplace_back(mousePos);
                    }
                }
            }

            if (tow1Inst[tow1Inst.size()-1].getPlacedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                sf::Vector2f snapPos = grid.getSnapPoint(mousePos, levelMap);
                tow1Inst[tow1Inst.size()-1].moveTower(snapPos);
            }

            if (tow1Inst[tow1Inst.size()-1].getRotatedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                float ang = tow1Inst[tow1Inst.size()-1].findAngle(mousePos);
                ang = (((int) (ang/45 + 1)) % 8)/2;
                tow1Inst[tow1Inst.size()-1].rotateTower(ang*90);
            }
        }

        window.draw(grid);

        for(int i=0; i<level.getSpawnPointNumber(); i++)
            if(spawnPoints[i].canGenerateEnemy())
                enemies.emplace_back(spawnPoints[i].getSpawnPointPosition(), spawnPoints[i].getCheckpoints());

        for(int i=0; i<(int) enemies.size(); i++)
        {
            enemies[i].updatePosition();
            enemies[i].drawEnemy(window);
            if(enemies[i].reachedObjective())
            {
                level.deductLives();
                enemies.erase(enemies.begin()+i);
                if(!level.getLives())
                {
                    level.levelLost(window);
                    std::cout<<"\n :(\n";
                }
            }
        }

        tow1.drawTower(window);
        for(int i=0; i<(int) tow1Inst.size(); i++)
        {
            if(tow1Inst[i].canShoot())
            {
                for(int j=0; j<(int) enemies.size(); j++)
                {
                    if(tow1Inst[i].isSet() && tow1Inst[i].checkAttackRange(enemies[j].getPosition()))
                    {
                        //tow1Inst[i].clockReset();
                        int ang = tow1Inst[i].findAngle(enemies[j].getPosition());
                        tow1Inst[i].rotateTower(ang);
                        projectiles.emplace_back(enemies[j].getPosition(), ang, tow1Inst[i], j);
                        break;
                    }

                }
            }
            tow1Inst[i].drawTower(window);
            //tow1Inst[i].drawAttackRange(window);
        }

        for(int i=0; i<(int) projectiles.size(); i++)
        {
            projectiles[i].moveProjectile(window);
            if(projectiles[i].reachedTarget())
            {
                if(enemies[projectiles[i].getTargetIndex()].isDead(projectiles[i].getDamage()))
                    enemies.erase(enemies.begin()+projectiles[i].getTargetIndex());
                projectiles.erase(projectiles.begin()+i);
            }
        }

        window.display();
    }

    return 0;
}
