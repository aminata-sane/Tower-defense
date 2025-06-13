#include <SFML/Graphics.hpp>
#include "game.h"
#include "ui.h"

int main() {
    // 1. Crée une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kingdom Defense");
    window.setFramerateLimit(60);

    // 2. Crée une instance du jeu
    Game game;
    UI ui;

    // 3. Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Gestion de la fermeture de la fenêtre
            if (event.type == sf::Event::Closed)
                window.close();

            // Gestion des clics souris ou autres événements utilisateur
            ui.handleEvent(event, window); //futur clics / boutons
        }

        // 4. Mise à jour de la logique du jeu
        game.update();
        ui.update(game.getPlayer()); // UI mise à jour en fonction de l’état du jeu

        // 5. Dessin de la scène
        window.clear();

        game.draw(window); // Dessine la carte, ennemis, tours, projectiles
        ui.draw(window);   // Dessine l’interface (score, boutons…)

        window.display();
    }

    return 0;
}
