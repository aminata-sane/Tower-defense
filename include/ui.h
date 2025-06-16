#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include "player.h"

class Player; // Déclaration anticipée si Player.h n'est pas inclus ici (mais il l'est dans game.h)

class UI {
public:
    UI();
    UI(Player& player); // AJOUTÉ : Constructeur prenant une référence à Player
    // Ou UI(const Player& player); si l'UI ne modifie pas le joueur directement

    void update(const Player& player); // Met à jour l'UI avec les infos du joueur
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::Event& event, sf::RenderWindow& window); // Signature de base

private:
    // Membres pour les éléments de l'UI (boutons, textes, etc.)
    sf::Font font;

    sf::Text goldText;
    sf::Text healthText;
    sf::Text scoreText;

    // Player& playerRef; // Optionnel: stocker une référence si nécessaire au-delà de update()

    void loadResources(); // Pour charger la police, etc.
};

#endif // UI_H
