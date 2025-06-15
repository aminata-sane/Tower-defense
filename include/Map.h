#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
public:
    bool loadFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);

    bool isPath(int x, int y) const;
    bool canPlaceTower(int x, int y) const;

    const std::vector<sf::Vector2i>& getSpawnPoints() const;
    const std::vector<sf::Vector2i>& getPath() const;
    sf::Vector2i getEndPoint() const;

private:
    sf::Texture mapTexture;
    sf::Sprite mapSprite;

private:
    std::vector<std::vector<int>> grid;
    std::vector<sf::Vector2i> pathTiles;
    std::vector<sf::Vector2i> spawnPoints;
    sf::Vector2i endPoint;

    int tileSize = 64;

    sf::Texture texturePath;
    sf::Texture textureBuild;
    sf::Texture textureEmpty;
    sf::Sprite sprite;

    void loadTextures();
};
