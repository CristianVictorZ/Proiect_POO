#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <memory>

#include "headers/towers.hpp"
#include "headers/grid.hpp"
#include "headers/projectiles.hpp"
#include "headers/enemies.hpp"
#include "headers/spawnpoint.hpp"
#include "headers/level.hpp"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Game Window", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(100);

    Level level;
    std::vector<SpawnPoint> spawnPoints;
    for(int i=0; i<level.getSpawnPointNumber(); i++)
    {
        spawnPoints.emplace_back(level.getSpawnPointPositions(i), level.getCheckpoints(i), level.getSpawnPointTimer(i), level.getSpawnPointEnemyNumber(i), level.getEnemyType(i));
        level.setEnemyNumber(spawnPoints[i].getEnemyNumber());
    }

    Tower_01 tow1(sf::Vector2f(1000.0f,840.0f));
    Tower_02 tow2(sf::Vector2f(900.0f,840.0f));
    std::vector<std::shared_ptr<Tower>> tow1Inst;
    tow1Inst.reserve(10);

    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<std::shared_ptr<Enemy>> enemies;

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
                    if((int) tow1Inst.size()!=0 && tow1Inst.back()->getPlacedStatus() && snapPos != mousePos)
                    {
                        tow1Inst.back()->hasBeenPlaced();
                    }else if((int) tow1Inst.size()!=0 && tow1Inst.back()->getRotatedStatus())
                    {
                        tow1Inst.back()->hasBeenRotated();
                        float ang = tow1Inst.back()->findAngle(mousePos);
                        ang = (((int) (ang/45 + 1)) % 8)/2;
                        tow1Inst.back()->rotateTower(ang*90);
                        tow1Inst.back()->hasBeenSet();
                    }else if(tow1.checkMousePosition(mousePos))
                    {
                        try {
                            tow1Inst.emplace_back(Tower_01(mousePos).clone());
                        }
                        catch(std::exception& e) {
                            std::cerr << e.what() << '\n';
                        }
                    }else if(tow2.checkMousePosition(mousePos))
                    {
                        try {
                            tow1Inst.emplace_back(Tower_02(mousePos).clone());
                        }
                        catch(std::exception& e) {
                            std::cerr << e.what() << '\n';
                        }
                    }
                }else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    if(!tow1Inst.back()->isSet())
                    {
                        tow1Inst.erase(tow1Inst.end());
                    }
                }
            }

            if ((int) tow1Inst.size()!=0 && tow1Inst.back()->getPlacedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                sf::Vector2f snapPos = grid.getSnapPoint(mousePos, levelMap);
                tow1Inst.back()->moveTower(snapPos);
            }

            if ((int) tow1Inst.size()!=0 && tow1Inst.back()->getRotatedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                float ang = tow1Inst[tow1Inst.size()-1]->findAngle(mousePos);
                ang = (((int) (ang/45 + 1)) % 8)/2;
                tow1Inst.back()->rotateTower(ang*90);
            }
        }

        window.draw(grid);

        for(int i=0; i<level.getSpawnPointNumber(); i++)
            if(spawnPoints[i].canGenerateEnemy())
            {
                if(spawnPoints[i].getEnemyType() == 1)
                    enemies.emplace_back(Enemy_01(spawnPoints[i].getSpawnPointPosition(), spawnPoints[i].getCheckpoints()).clone());
                else if(spawnPoints[i].getEnemyType() == 2)
                    enemies.emplace_back(Enemy_02(spawnPoints[i].getSpawnPointPosition(), spawnPoints[i].getCheckpoints()).clone());
            }

        tow1.drawTower(window);
        tow2.drawTower(window);
        for(int i=0; i<(int) tow1Inst.size(); i++)
        {
            tow1Inst[i]->update(enemies, projectiles, tow1Inst[i], window);
        }
        for(int i=0; i<(int) projectiles.size(); i++)
        {
            projectiles[i]->moveProjectile(window);
            if(projectiles[i]->reachedTarget())
            {
                if(enemies[projectiles[i]->getTargetIndex()]->isDead(projectiles[i]->getDamage()))
                {
                    enemies[i]->enemyRemove(enemies, projectiles, projectiles[i]->getTargetIndex());

                    level.enemyDefeated();
                    if(level.levelWon(window))
                        std::cout<<"\n :)\n";
                }
                projectiles.erase(projectiles.begin()+i);
            }
        }

        for(int i=0; i<(int) enemies.size(); i++)
        {
            enemies[i]->updatePosition();
            enemies[i]->drawEnemy(window);
            if(enemies[i]->reachedObjective())
            {
                level.deductLives();

                enemies[i]->enemyRemove(enemies, projectiles, i);

                level.enemyDefeated();
                if(!level.getLives())
                {
                    level.levelLost(window);
                    std::cout<<"\n :(\n";
                }
            }
        }

        window.display();
    }

    return 0;
}
