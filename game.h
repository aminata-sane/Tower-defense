#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"
#include "tower.h"
#include "projectile.h"
#include "player.h"
#include "map.h"

class Game {
public:
    Game();                      // Constructeur
    void update();               // Met à jour les objets du jeu
    void draw(sf::RenderWindow& window); // Dessine les objets sur l'écran

    Player& getPlayer();

private:
    Map map;                     // Carte du jeu
    Player player;               // Joueur (or, vies, score)
    std::vector<Enemy> enemies;  // Liste des ennemis
    std::vector<Tower> towers;   // Liste des tours
    std::vector<Projectile> projectiles; // Projectiles tirés

    sf::Clock waveTimer;         // Chrono pour les vagues
    void spawnWave();            // Fonction pour lancer une vague
};

#endif
