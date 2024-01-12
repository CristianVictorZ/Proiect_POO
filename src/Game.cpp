#include "../headers/Game.hpp"

int Game::run(sf::RenderWindow &window)
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(100);

    ///Initializare EnemyFactory.
    std::shared_ptr<EnemyFactory> enemyfactory01 = std::make_shared<Enemy_01Factory>();
    std::shared_ptr<EnemyFactory> enemyfactory02 = std::make_shared<Enemy_02Factory>();

    ///Initializare Level.
    Level level;

    ///Initializare SpawnPoints.
    std::vector<SpawnPoint> spawnPoints;
    for(int i=0; i<level.getSpawnPointNumber(); i++)
    {
        spawnPoints.emplace_back(level.getSpawnPointPositions(i), level.getCheckpoints(i), level.getSpawnPointTimer(i), level.getSpawnPointEnemyNumber(i), level.getEnemyType(i));
        level.setEnemyNumber(spawnPoints[i].getEnemyNumber());
    }

    ///Initializare Turnuri.
    Tower_01 tow1(sf::Vector2f(1000.0f,840.0f));
    Tower_02 tow2(sf::Vector2f(900.0f,840.0f));
    std::vector<std::shared_ptr<Tower>> towers;
    towers.reserve(10);

    ///Initializare Observer.
    std::vector<std::shared_ptr<EnemyObserver>> enmObs;
    Subject obs;

    ///Initializare Projectiles.
    std::vector<std::shared_ptr<Projectile>> projectiles;

    ///Initializare Enemies.
    std::vector<std::shared_ptr<Enemy>> enemies;

    ///Initialicare Grid.
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

    ///Rulare.
    while (window.isOpen())
    {
        window.clear();
        sf::Event event;

        ///Verificare event-uri.
        while (window.pollEvent(event))
        {
            ///Inchidere fereastra.
            if (event.type == sf::Event::Closed)
                window.close();

            ///Verificari mouse click-uri.
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = sf::Vector2f((float) event.mouseButton.x, (float) event.mouseButton.y);
                    sf::Vector2f snapPos = grid.getSnapPoint(mousePos, levelMap);
                    if((int) towers.size()!=0 && towers.back()->getPlacedStatus() && snapPos != mousePos)///Daca turnul a fost pus jos.
                    {
                        towers.back()->hasBeenPlaced();
                    }else if((int) towers.size()!=0 && towers.back()->getRotatedStatus())///Daca turnul a fost pus jos si se selecteaza rotatia acestuia.
                    {
                        towers.back()->hasBeenRotated();
                        float ang = towers.back()->findAngle(mousePos);
                        ang = (((int) (ang/45 + 1)) % 8)/2;
                        towers.back()->rotateTower(ang*90);
                        towers.back()->hasBeenSet();
                    }else if(tow1.checkMousePosition(mousePos))///Verifica daca a fost selectat tow1.
                    {
                        try {
                            towers.emplace_back(Tower_01(mousePos).clone());///Introduce in vectorul de turnuri.
                            ///Introduce in clasele observer-ului
                            enmObs.emplace_back(EnemyObserver(towers.back()).clone());
                            obs.registerObserver(enmObs.back());
                        }
                        catch(std::exception& e) {
                            std::cerr << e.what() << '\n';
                        }
                    }else if(tow2.checkMousePosition(mousePos))///Verifica daca a fost selectat tow2.
                    {
                        try {
                            towers.emplace_back(Tower_02(mousePos).clone());///Introduce in vectorul de turnuri.
                            ///Introduce in clasele observer-ului
                            enmObs.emplace_back(EnemyObserver(towers.back()).clone());
                            obs.registerObserver(enmObs.back());
                        }
                        catch(std::exception& e) {
                            std::cerr << e.what() << '\n';
                        }
                    }
                }else if(event.mouseButton.button == sf::Mouse::Right)///Sterge turnul daca acesta inca nu a fost setat si se apasa clic dreapta.
                {
                    if(!towers.back()->isSet())
                    {
                        towers.erase(towers.end());
                    }
                }
            }

            ///Daca turnul inca nu a fost pus, acesta se va deplasa odata cu mouse-ul pana la umratorul clic stanga.
            if ((int) towers.size()!=0 && towers.back()->getPlacedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                sf::Vector2f snapPos = grid.getSnapPoint(mousePos, levelMap);
                towers.back()->moveTower(snapPos);
            }

            ///Daca turnul a fost pus, dar nu si rotit, acesta se va roti odata cu mouse-ul pana la umratorul clic stanga.
            if ((int) towers.size()!=0 && towers.back()->getRotatedStatus() && event.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos = sf::Vector2f((float) event.mouseMove.x, (float) event.mouseMove.y);
                float ang = towers.back()->findAngle(mousePos);
                ang = (((int) (ang/45 + 1)) % 8)/2;
                towers.back()->rotateTower(ang*90);
            }
        }

        window.draw(grid);///Deseneaza grid-ul.

        ///Genereaza inamicii.
        for(int i=0; i<level.getSpawnPointNumber(); i++)
            if(spawnPoints[i].canGenerateEnemy())
            {
                if(spawnPoints[i].getEnemyType() == 1)
                    enemies.emplace_back(enemyfactory01->createEnemy(spawnPoints[i].getSpawnPointPosition(), spawnPoints[i].getCheckpoints()));
                else if(spawnPoints[i].getEnemyType() == 2)
                    enemies.emplace_back(enemyfactory02->createEnemy(spawnPoints[i].getSpawnPointPosition(), spawnPoints[i].getCheckpoints()));
            }

        ///Deseneaza turnurile de sub grid.
        tow1.drawTower(window);
        tow2.drawTower(window);

        ///Functia de update atunci cand nu se foloseste observer-ul.
//        for(int i=0; i<(int) towers.size(); i++)
//        {
//            towers[i]->update(enemies, projectiles, towers[i], window);
//        }

        ///Actualizeaza proiectilele trase de turnuri.
        for(int i=0; i<(int) projectiles.size(); i++)
        {
            projectiles[i]->moveProjectile(window);
            if(projectiles[i]->reachedTarget())
            {
                ///Cand proiectilul isi atinge tinta, verifica daca se scade viata inamicului sau daca acesta este sters, in functie de cata viata mai are.
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

        ///Actualizeaza inamicii.
        for(int i=0; i<(int) enemies.size(); i++)
        {
            sf::Vector2f ps = enemies[i]->updatePosition();

            ///Functii necesare pentru rularea observer-ului;
            ///Pentru functionarea oberver-ului turnurile trebuie actualizate de fiecare data cand un inamic se deplaseaza; mai ineficient decat fara observer.
            obs.setPosition(ps,i);
            for(int j=0; j<(int) towers.size(); j++)
            {
                towers[j]->update(projectiles, towers[j], window);
            }

            ///Deseneaza un inamic
            enemies[i]->drawEnemy(window);

            ///Verifica daca inamicii au ajuns la obiectiv
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
