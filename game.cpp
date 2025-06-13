#include "game.h"
#include <iostream>

Game::Game() {
    // Initialisation du joueur, de la carte, etc.
    player = Player(100, 50); // 100 points de vie, 50 or
    map.load();               // Charge la carte (défini dans map.cpp)

    // Première vague
    spawnWave();
}

void Game::update() {
    // Met à jour les ennemis
    for (auto& enemy : enemies)
        enemy.update();

    // Met à jour les tours
    for (auto& tower : towers)
        tower.update(enemies, projectiles); // tours cherchent une cible

    // Met à jour les projectiles
    for (auto& proj : projectiles)
        proj.update();

    // Gérer la fin de vague
    if (waveTimer.getElapsedTime().asSeconds() > 10) {
        spawnWave();
        waveTimer.restart();
    }
}

void Game::draw(sf::RenderWindow& window) {
    map.draw(window); // Affiche la carte

    for (auto& tower : towers)
        tower.draw(window);

    for (auto& enemy : enemies)
        enemy.draw(window);

    for (auto& proj : projectiles)
        proj.draw(window);

    // Player ou interface dessinée ailleurs (UI)
}

void Game::spawnWave() {
    // Exemple : ajoute 5 ennemis à la vague
    for (int i = 0; i < 5; ++i) {
        enemies.push_back(Enemy(/* position de départ */));
    }

}

Player& Game::getPlayer() {
    return player;
}
