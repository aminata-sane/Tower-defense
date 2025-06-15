#include <SFML/Graphics.hpp>
#include "Map.h"
#include "wave.h"

enum class GameState {
    Map1, Map2, Victory, Defeat
};

int main() {
    GameState state = GameState::Map1;
    std::string mapFile = "assets/maps1.txt";

    Map map;
    if (!map.loadFromFile(mapFile))
        return -1;

    WaveManager* wave = new WaveManager(map);
    wave->setMaxEnemiesAllowed(10);

    sf::RenderWindow window(sf::VideoMode(960, 768), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");
    sf::Text text("", font, 50);
    text.setPosition(260, 300);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (state == GameState::Map1 || state == GameState::Map2) {
            wave->update(dt.asSeconds());

            if (wave->hasLost()) {
                state = GameState::Defeat;
                text.setString("You Lost!");
                text.setFillColor(sf::Color::Red);
            } else if (wave->hasWon()) {
                if (state == GameState::Map1) {
                    delete wave;

                    state = GameState::Map2;
                    mapFile = "assets/maps2.txt";
                    map.loadFromFile(mapFile);

                    wave = new WaveManager(map);
                    wave->setMaxEnemiesAllowed(5);
                } else {
                    state = GameState::Victory;
                    text.setString("Victory!");
                    text.setFillColor(sf::Color::Green);
                    delete wave;
                    wave = nullptr;
                }
            }
        }

        window.clear();
        map.draw(window);
        if (wave) wave->draw(window);
        if (state == GameState::Defeat || state == GameState::Victory)
            window.draw(text);
        window.display();
    }

    delete wave;
    return 0;
}
