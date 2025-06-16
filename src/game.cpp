// #include "game.h"
// #include <iostream>

// Game::Game()
// {
//     // Initialisation du joueur, de la carte, etc.
//     player = Player(100, 50); // 100 points de vie, 50 or
//     map.load();               // Charge la carte (défini dans map.cpp)

//     // Première vague
//     spawnWave();
// }

// void Game::update()
// {
//     // Met à jour les ennemis
//     for (auto &enemy : enemies)
//         enemy.update();

//     // Met à jour les tours
//     for (auto &tower : towers)
//         tower.update(enemies, projectiles); // tours cherchent une cible

//     // Met à jour les projectiles
//     for (auto &proj : projectiles)
//         proj.update();

//     // Gérer la fin de vague
//     if (waveTimer.getElapsedTime().asSeconds() > 10)
//     {
//         spawnWave();
//         waveTimer.restart();
//     }
// }

// void Game::draw(sf::RenderWindow &window)
// {
//     map.draw(window); // Affiche la carte

//     for (auto &tower : towers)
//         tower.draw(window);

//     for (auto &enemy : enemies)
//         enemy.draw(window);

//     for (auto &proj : projectiles)
//         proj.draw(window);

//     // Player ou interface dessinée ailleurs (UI)
// }

// void Game::spawnWave()
// {
//     // Exemple : ajoute 5 ennemis à la vague
//     for (int i = 0; i < 5; ++i)
//     {
//         enemies.push_back(EnemyFactory::createEnemy(...));
//     }
// }

// Player &Game::getPlayer()
// {
//     return player;
// }

#include "game.h"
#include "enemyFactory.h"
#include <iostream>

// Constructor
Game::Game()
    : map(), waveManager(map), player(100, 50) // 100 HP, 50 gold
{
    if (!map.loadFromFile("assets/maps/maps1.txt"))
    {
        std::cerr << "Failed to load map!" << std::endl;
    }

    waveManager.setMaxEnemiesAllowed(10);
}

// Update game logic
void Game::update(float deltaTime)
{
    waveManager.update(deltaTime);

    // Update towers (each tower chooses target and shoots)
    for (auto &tower : towers)
    {
        tower->update(deltaTime, waveManager.getEnemies(), projectiles, playerGold);
    }

    // Update projectiles
    for (auto &proj : projectiles)
    {
        proj->update(deltaTime);
    }

    // Remove inactive projectiles
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [](const std::shared_ptr<Projectile> &p)
                                     {
                                         return !p->isActive();
                                     }),
                      projectiles.end());

    // Reward player for killed enemies
    auto &enemies = waveManager.getEnemies();
    for (Enemy *enemy : enemies)
    {
        if (enemy->isDead())
        {
            player.addGold(enemy->getReward());
            player.addScore(10);
        }
    }
}

// Draw game scene
void Game::draw(sf::RenderWindow &window)
{
    map.draw(window);

    for (auto &tower : towers)
        tower->draw(window);

    for (Enemy *enemy : waveManager.getEnemies())
        enemy->draw(window);

    for (auto &proj : projectiles)
        proj->draw(window);
}

// Place a tower at a given tile
bool Game::placeTower(sf::Vector2i gridPosition, TowerType type)
{
    if (!map.canPlaceTower(gridPosition.x, gridPosition.y))
        return false;

    sf::Vector2f pixelPosition(gridPosition.x * 64 + 32, gridPosition.y * 64 + 32);

    std::shared_ptr<Tower> newTower = std::make_shared<Tower>(type, pixelPosition);
    int cost = newTower->getCost();

    if (player.spendGold(cost))
    {
        towers.push_back(newTower);
        return true;
    }
    return false;
}

Player &Game::getPlayer()
{
    return player;
}
