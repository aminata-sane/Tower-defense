#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory> // Pour std::unique_ptr

// Inclusions des définitions complètes des classes utilisées par Game
#include "enemy.h"
#include "tower.h"      // Doit définir TowerType (par exemple, enum class TowerType)
#include "projectile.h"
#include "player.h"
#include "Map.h"
#include "GameState.h"  // Doit définir enum class GameState
#include "wave.h"       // Doit définir WaveManager
#include "ui.h"         // Doit définir UI

class Game {
public:
    Game();
    void update(float dt);
    void draw(sf::RenderWindow& window);

    Player& getPlayer();
    Map& getMap(); // Si Game gère sa propre carte
    bool isGameOver() const;
    GameState getCurrentState() const;

    void placeTower(TowerType type, sf::Vector2i tilePosition);
    void handleInput(const sf::Event& event, sf::RenderWindow& window);

    // AJOUTÉ (si cette méthode est toujours nécessaire et utilisée par Game)
    // Si WaveManager ajoute directement des ennemis au vecteur 'enemies' de Game,
    // cette méthode publique pourrait ne pas être nécessaire.
    // Vérifie son utilité avant de la garder.
    void addEnemy(std::shared_ptr<Enemy> enemy);   // NOUVEAU (si tu l'avais)


private:
    Map gameMap; // Game gère sa propre instance de carte
    Player player;
    UI gameUI;
    std::unique_ptr<WaveManager> waveManager;

    std::vector<std::shared_ptr<Enemy>> enemies;   // NOUVEAU
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    GameState currentState;
    sf::Text endScreenText;
    sf::Font font;

    void loadResources();
    void updateEnemies(float dt);
    void updateTowers(float dt);
    void updateProjectiles(float dt);
    void checkCollisions();
    void cleanupEntities();
    void updateGameState(); // Gère la logique de victoire/défaite et potentiellement le changement de carte
};

#endif // GAME_H
