#include "game.h"
#include "wave.h"       // Pour WaveManager
#include "ui.h"         // Pour UI
#include "projectile.h" // Pour Projectile class
#include "enemy.h"      // Pour Enemy class
#include "player.h"     // Pour Player class
#include "tower.h"      // Pour TowerType et Tower class
#include "Map.h"        // Pour Map class
#include "GameState.h"  // Pour GameState enum

// Inclusions pour les types d'ennemis concrets si Game::addEnemy les reçoit
// ou si WaveManager a besoin de les connaître et que Game les inclut par transitivité.
// Idéalement, Game n'a pas besoin de connaître les types concrets si WaveManager les crée.
// #include "Goblin.h"
// #include "Orc.h"
// #include "Boss.h"

#include <iostream>
#include <algorithm> // Pour std::remove_if
#include <cmath>     // Pour std::sqrt dans les calculs de distance (si utilisé)

// Constructeur
Game::Game() : player(100, 150), // Santé et or initiaux du joueur
               gameMap(),      // S'assure que gameMap est initialisée (charge une carte par défaut ou est vide)
               gameUI(player), // Initialise l'UI avec une référence au joueur
               currentState(GameState::LOADING) {

    if (!gameMap.loadFromFile("assets/maps/default_map.txt")) { // Exemple de chargement
        std::cerr << "Erreur Game: Impossible de charger la carte par défaut gameMap." << std::endl;
        // Gérer l'erreur, peut-être passer à un état d'erreur ou utiliser une carte vide
    }

    // Initialiser WaveManager (exemple, WaveManager pourrait prendre la carte pour les chemins)
    waveManager = std::make_unique<WaveManager>(gameMap);

    loadResources();
    currentState = GameState::PLAYING; // Ou GameState::MENU si tu as un menu
}

void Game::loadResources() {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) { // Remplace par le chemin de ta police
        std::cerr << "Erreur Game: Impossible de charger la police." << std::endl;
        // Gérer l'erreur
    }
    endScreenText.setFont(font);
    endScreenText.setCharacterSize(50);
    endScreenText.setFillColor(sf::Color::White);
    // Le positionnement se fera dans draw ou updateGameState
}

// Boucle de mise à jour principale pour la logique du jeu
void Game::update(float dt) {
    if (currentState != GameState::PLAYING) {
        return; // Ne rien mettre à jour si le jeu n'est pas en cours
    }

    if (waveManager) {
        waveManager->update(dt, enemies); // WaveManager met à jour ses vagues et ajoute des ennemis à la liste `enemies`
    }

    updateTowers(dt);
    updateEnemies(dt);
    updateProjectiles(dt);
    checkCollisions();
    cleanupEntities();
    updateGameState(); // Vérifier les conditions de victoire/défaite
}

// Dessine tous les éléments de jeu
void Game::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(20, 20, 20)); // Fond sombre

    gameMap.draw(window); // Dessine la carte

    for (const auto& tower : towers) {
        if(tower) tower->draw(window);
    }

    for (const auto& enemy : enemies) {
        if(enemy) enemy->draw(window);
    }

    for (const auto& projectile : projectiles) {
        if(projectile && projectile->isActive()) {
            projectile->draw(window);
        }
    }

    gameUI.update(player); // Met à jour l'UI avec les infos du joueur
    gameUI.draw(window);   // Dessine l'UI

    if (currentState == GameState::VICTORY || currentState == GameState::DEFEAT) {
        // Centrer le texte (exemple simple)
        sf::FloatRect textRect = endScreenText.getLocalBounds();
        endScreenText.setOrigin(textRect.left + textRect.width / 2.0f,
                                textRect.top + textRect.height / 2.0f);
        endScreenText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
        window.draw(endScreenText);
    }
    // window.display() est appelé dans main.cpp
}

void Game::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (currentState == GameState::PLAYING) {
        // Exemple: Placer une tour au clic de souris
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                sf::Vector2i tilePos = gameMap.worldToTileCoords(window.mapPixelToCoords(mousePixelPos)); // Convertir en coordonnées de tuile

                // TODO: Permettre à l'UI de sélectionner le type de tour
                // Pour l'instant, plaçons un type par défaut si la tuile est valide
                if (gameMap.isTileValid(tilePos.x, tilePos.y) /*&& gameMap.isTileBuildable(tilePos)*/) {
                     // placeTower(TowerType::ARCHER, tilePos); // Décommente et adapte avec ton TowerType
                }
            }
        }
    }
    // TODO: Gérer les entrées pour le menu, la pause, etc.
    // gameUI.handleEvent(event, window); // Si l'UI gère ses propres événements
}


Player& Game::getPlayer() {
    return player;
}

Map& Game::getMap() {
    return gameMap;
}

bool Game::isGameOver() const {
    return currentState == GameState::VICTORY || currentState == GameState::DEFEAT;
}

GameState Game::getCurrentState() const {
    return currentState;
}

// MODIFIE CETTE MÉTHODE
void Game::addEnemy(std::shared_ptr<Enemy> enemy) { // Doit correspondre à game.h
    if (enemy) {
        // Si enemies est std::vector<std::shared_ptr<Enemy>>, ceci est correct.
        // std::move n'est pas strictement nécessaire pour un shared_ptr passé par valeur
        // s'il doit être copié dans le vecteur, mais ne nuit pas.
        // Si enemy était une rvalue reference (std::shared_ptr<Enemy>&&), std::move serait plus pertinent.
        enemies.push_back(enemy); 
    }
}

void Game::placeTower(TowerType type, sf::Vector2i tilePosition) {
    // 1. Convertir tilePosition en coordonnées du monde (centre de la tuile)
    float tileSize = gameMap.getTileSize(); // Supposant que Map a getTileSize()
    sf::Vector2f worldPosition(
        static_cast<float>(tilePosition.x) * tileSize + tileSize / 2.0f,
        static_cast<float>(tilePosition.y) * tileSize + tileSize / 2.0f
    );

    // 2. Vérifier si le joueur a assez d'argent, si l'emplacement est valide, etc.
    //    Cette logique peut être plus complexe et impliquer Tower::getCost(type)
    //    et gameMap.isTileBuildable(tilePosition)
    //    Pour l'instant, une vérification simple de l'or :
    unsigned int cost = 0; // Obtenir le coût en fonction du type
    // switch(type) { case TowerType::ARCHER: cost = 50; break; /* ... */ }
    // if (!player.canAfford(cost) || !gameMap.isTileBuildable(tilePosition)) {
    //     std::cout << "Impossible de placer la tour (or ou emplacement)." << std::endl;
    //     return;
    // }


    std::unique_ptr<Tower> newTower;
    // Remplace par tes vrais noms de classes de tours et TowerType
    // switch (type) {
    //     case TowerType::ARCHER:
    //         newTower = std::make_unique<ArcherTower>(worldPosition);
    //         cost = ArcherTower::getStaticCost(); // Supposant une méthode statique pour le coût
    //         break;
    //     case TowerType::CANON:
    //         // newTower = std::make_unique<CanonTower>(worldPosition);
    //         // cost = CanonTower::getStaticCost();
    //         break;
    //     default:
    //         std::cerr << "Type de tour inconnu pour placeTower." << std::endl;
    //         return;
    // }

    // Vérification finale du coût après avoir déterminé le type
    if (newTower && player.spendGold(cost)) { // spendGold retourne true si réussi
        towers.push_back(std::move(newTower));
        // gameMap.setTileOccupied(tilePosition, true); // Marquer la tuile
        std::cout << "Tour placée. Or restant: " << player.getGold() << std::endl;
    } else {
        std::cout << "Placement de tour échoué (pas assez d'or ou type invalide)." << std::endl;
    }
}

// --- Méthodes privées d'aide ---

void Game::updateEnemies(float dt) {
    for (auto& enemy : enemies) {
        if (enemy && !enemy->isDead()) {
            enemy->update(dt);

            if (enemy->reachedGoal()) {
                player.takeDamage(enemy->getDamageToPlayer()); // Supposant que Enemy a getDamageToPlayer()
                std::cout << "Un ennemi a atteint le but! Santé du joueur: " << player.getHealth() << std::endl;
                // MODIFIÉ ICI: Utilise getCurrentHealth() au lieu de getHp()
                enemy->takeDamage(enemy->getCurrentHealth() + 1); // S'assure qu'il est marqué comme mort
            }
        }
    }
}

void Game::updateTowers(float dt) {
    for (auto& tower : towers) {
        if(tower) {
            // La signature de Tower::update doit être :
            // void update(float dt, const std::vector<std::unique_ptr<Enemy>>& all_enemies,
            //             std::vector<std::unique_ptr<Projectile>>& game_projectiles, Player& game_player);
            tower->update(dt, enemies, projectiles, player);
        }
    }
}

void Game::updateProjectiles(float dt) {
    for (auto& projectile : projectiles) {
        if (projectile && projectile->isActive()) {
            projectile->update(dt);
        }
    }
}

void Game::checkCollisions() {
    for (auto& projectile : projectiles) {
        if (!projectile || !projectile->isActive()) {
            continue;
        }

        for (auto& enemy : enemies) {
            if (!enemy || enemy->isDead()) {
                continue;
            }

            // Vérification de collision (exemple simple basé sur les rectangles englobants)
            // Tu devras peut-être ajuster cela avec des cercles ou des formes plus précises.
            if (projectile->getBounds().intersects(enemy->getBounds())) {
                enemy->takeDamage(projectile->getDamage());
                projectile->setActive(false); // Le projectile est consommé

                if (enemy->isDead()) {
                    player.addGold(enemy->getGoldValue()); // Supposant que Enemy a getGoldValue()
                    // player.addScore(enemy->getScoreValue());
                }
                break; // Le projectile a touché un ennemi, passe au projectile suivant
            }
        }
    }
}

void Game::cleanupEntities() {
    // Nettoyer les ennemis morts
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::shared_ptr<Enemy>& e) {
                                     return e->isDead() || e->reachedGoal(); // Ou toute autre condition de suppression
                                 }),
                  enemies.end());

    // Nettoyer les projectiles inactifs
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [](const std::unique_ptr<Projectile>& p) {
                                         return !p->isActive();
                                     }),
                      projectiles.end());

    // Les tours sont généralement retirées par une action du joueur, pas automatiquement ici
}

void Game::updateGameState() {
    if (currentState != GameState::PLAYING) return;

    if (!player.isAlive()) {
        currentState = GameState::DEFEAT;
        endScreenText.setString("DEFAITE !");
        std::cout << "Jeu terminé : Défaite." << std::endl;
    } else if (waveManager && waveManager->allWavesCompleted() && enemies.empty()) {
        // Vérifier si toutes les vagues sont terminées ET qu'il n'y a plus d'ennemis à l'écran
        currentState = GameState::VICTORY;
        endScreenText.setString("VICTOIRE !");
        std::cout << "Jeu terminé : Victoire." << std::endl;
    }
}



