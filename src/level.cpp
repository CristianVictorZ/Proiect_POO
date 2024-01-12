#include "../headers/level.hpp"

///Constructor.
Level::Level(){}

///Destructor.
Level::~Level(){}

///Intoarce numarul de spawnpoint-uri
int Level::getSpawnPointNumber()
{
    return spawnPointNumber;
}

///Intoarce tipul inamicilor generati de un spawnpoint.
int Level::getEnemyType(int k)
{
    return spawnPointEnemyTypes[k];
}

///Intoarce numarul de inamici generati de un sapwnpoint.
int Level::getSpawnPointEnemyNumber(int k)
{
    return spawnPointEnemyNumber[k];
}

///Intoarce locatia unui spawnpoint.
sf::Vector2f Level::getSpawnPointPositions(int k)
{
    return spawnPointPositions[k];
}

///Intoarce vectorul de checkpoints care ii corespunde unui spawnpoint.
std::vector<sf::Vector2f> Level::getCheckpoints(int k)
{
    return checkpoints[k];
}

///Intoarce timpul dupa care un spawnpoint genereaza un inamic.
sf::Time Level::getSpawnPointTimer(int k)
{
    return spawnPointTimers[k];
}

///Calculeaza numarul toatal de inamici din nivel.
void Level::setEnemyNumber(int _enemyNumber)
{
    enemyNumber += _enemyNumber;
}

///Scade numarul de vieti.
void Level::deductLives()
{
    lives--;
}

///Inchide fereastra cand nivelul este pierdut.
void Level::levelLost (sf::RenderWindow &window)
{
    window.close();
}

///Creste numarul de inamici invinsi.
void Level::enemyDefeated()
{
    defeatedEnemies++;
}

///Inchide fereastra cand nivelul este castigat.
bool Level::levelWon (sf::RenderWindow &window)
{
    if(enemyNumber == defeatedEnemies)
    {
        window.close();
        return 1;
    }
    return 0;
}

///Intoarce numarul ramas de vieti.
int Level::getLives()
{
    return lives;
}
