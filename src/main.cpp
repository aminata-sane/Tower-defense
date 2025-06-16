#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <memory> // Pour std::unique_ptr

// Inclusions des fichiers du projet
#include "Map.h"        // Doit avoir une méthode draw(sf::RenderWindow& window)
#include "game.h"       // Game inclut GameState.h, Player.h, etc.
#include "ui.h"         // Doit avoir draw(sf::RenderWindow& window) et update(const Player&)
#include "wave.h"       // Pour WaveManager
// Les inclusions spécifiques d'ennemis (Boss, Goblin, Orc) et EnemyFactory
// sont pertinentes si main les utilise directement, sinon Game/WaveManager les gère.

// Utiliser le GameState défini dans GameState.h (inclus via game.h)
// Si GameState.h n'est pas inclus par game.h, il faut l'inclure ici.
// #include "GameState.h"


int main() {
    // Utilise le GameState de GameState.h, qui est accessible via Game
    // Pour la logique de haut niveau de main, tu peux avoir un état local
    // mais il est préférable de s'appuyer sur game.getCurrentState() si possible.
    // Pour cet exemple, je garde un état local simple pour la gestion des cartes.
    enum class MainLoopState { Map1, Map2, Victory, Defeat, Exit };
    MainLoopState currentMainState = MainLoopState::Map1;

    std::string mapFile = "assets/maps/maps1.txt"; // Assure-toi que le chemin est correct

    sf::RenderWindow window(sf::VideoMode(960, 768), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police.\n";
        return -1;
    }

    sf::Text endScreenText("", font, 50);

    Game game; // L'objet Game gère sa propre carte, son joueur, son UI interne, son waveManager

    sf::Clock clock;

    // Boucle principale
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            game.handleInput(event, window); // Game gère ses propres entrées
        }

        // Mise à jour de la logique de jeu
        // La logique de changement de carte et de victoire/défaite devrait idéalement être gérée par Game.
        // Pour l'instant, on se base sur game.isGameOver() et game.getCurrentState()
        
        game.update(dt.asSeconds());

        // Vérifier l'état du jeu après la mise à jour
        if (game.isGameOver()) {
            if (game.getCurrentState() == GameState::VICTORY) { // Supposant que GameState est dans game.h
                // Gérer la victoire (potentiellement passer à la carte suivante ou afficher l'écran de victoire final)
                // Cette logique complexe de changement de carte devrait être dans Game::updateGameState ou une méthode similaire.
                // Pour l'instant, affichons simplement un message si c'est une victoire finale.
                if (currentMainState != MainLoopState::Victory) { // Évite de reconfigurer à chaque frame
                    endScreenText.setString("VICTOIRE FINALE !");
                    endScreenText.setFillColor(sf::Color::Green);
                    sf::FloatRect textRect = endScreenText.getLocalBounds();
                    endScreenText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
                    endScreenText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
                    currentMainState = MainLoopState::Victory;
                }
            } else if (game.getCurrentState() == GameState::DEFEAT) {
                 if (currentMainState != MainLoopState::Defeat) { // Évite de reconfigurer à chaque frame
                    endScreenText.setString("DEFAITE !");
                    endScreenText.setFillColor(sf::Color::Red);
                    sf::FloatRect textRect = endScreenText.getLocalBounds();
                    endScreenText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
                    endScreenText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
                    currentMainState = MainLoopState::Defeat;
                }
            }
        }


        // Rendu
        window.clear(sf::Color(20, 20, 20)); // Fond sombre

        game.draw(window); // Game dessine tous ses composants (carte, tours, ennemis, projectiles, UI du jeu)

        // Dessiner le texte de fin de jeu global si nécessaire
        if (currentMainState == MainLoopState::Victory || currentMainState == MainLoopState::Defeat) {
            window.draw(endScreenText);
        }

        window.display();
    }

    return 0;
}
// La définition de Game::addEnemy a été supprimée d'ici. Elle doit être dans game.cpp.
