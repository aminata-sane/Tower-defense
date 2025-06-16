#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"
#include "tower.h"
#include "projectile.h"
#include "player.h"
#include "map.h"
#include "wave.h"

class Game
{
public:
    Game();                              // Constructeur
    void update(float deltaTime);        // Met à jour les objets du jeu
    void draw(sf::RenderWindow &window); // Dessine les objets sur l'écran

    bool placeTower(sf::Vector2i gridPosition, TowerType type);

    Player &getPlayer();

private:
    Map map;       // Carte du jeu
    Player player; // Joueur (or, vies, score)
    std::vector<Enemy *> enemies;
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    WaveManager waveManager;
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    int playerGold = 50;
    Map map;
    Player player;

    sf::Clock waveTimer; // Chrono pour les vagues
    void spawnWave();    // Fonction pour lancer une vague
};

#endif