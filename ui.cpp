#include "ui.h"
#include <iostream>

UI::UI() {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Erreur : impossible de charger la police." << std::endl;
    }

    goldText.setFont(font);
    goldText.setCharacterSize(20);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(10, 10);

    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition(10, 40);

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 70);
}

void UI::update(const Player& player) {
    goldText.setString("Or : " + std::to_string(player.getGold()));
    healthText.setString("Vies : " + std::to_string(player.getHealth()));
    scoreText.setString("Score : " + std::to_string(player.getScore()));
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(goldText);
    window.draw(healthText);
    window.draw(scoreText);
}

void UI::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Plus tard : interagir avec les boutons
}
