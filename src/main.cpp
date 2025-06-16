#include <SFML/Graphics.hpp>
#include "game.h"
#include "ui.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kingdom Defense");
    window.setFramerateLimit(60);

    Game game;
    UI ui;

    sf::Clock deltaClock;

    // Selected tower type (default: MELEE)
    TowerType selectedType = TowerType::MELEE;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Keyboard tower type selection
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Num1)
                    selectedType = TowerType::MELEE;
                else if (event.key.code == sf::Keyboard::Num2)
                    selectedType = TowerType::ARROW;
                else if (event.key.code == sf::Keyboard::Num3)
                    selectedType = TowerType::FIRE;
            }

            // Mouse click for tower placement
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i gridPos(mousePos.x / 64, mousePos.y / 64);
                if (game.placeTower(gridPos, selectedType))
                    std::cout << "Tower placed at: " << gridPos.x << "," << gridPos.y << std::endl;
            }

            // Future: UI button handling
            ui.handleEvent(event, window);
        }

        float deltaTime = deltaClock.restart().asSeconds();

        game.update(deltaTime);
        ui.update(game.getPlayer());

        window.clear();
        game.draw(window);
        ui.draw(window);
        window.display();
    }

    return 0;
}
