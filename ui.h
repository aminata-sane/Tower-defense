#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include "player.h"

class UI {
public:
    UI();

    void update(const Player& player);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window); // pour les boutons plus tard

private:
    sf::Font font;

    sf::Text goldText;
    sf::Text healthText;
    sf::Text scoreText;

    // Plus tard : des boutons SFML
};

#endif
