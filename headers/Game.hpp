#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <memory>

#include "towers.hpp"
#include "grid.hpp"
#include "projectiles.hpp"
#include "enemies.hpp"
#include "spawnpoint.hpp"
#include "level.hpp"
#include "observer.hpp"

class Game
{
public:
    int run(sf::RenderWindow &window);
};

