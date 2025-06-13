#include "Map.h"
#include <fstream>
#include <sstream>

bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    grid.clear();
    pathTiles.clear();
    spawnPoints.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int value;
        int x = 0;
        while (iss >> value) {
            row.push_back(value);
            int y = grid.size();

            if (value == 1)
                pathTiles.emplace_back(x, y);
            else if (value == 3)
                spawnPoints.emplace_back(x, y);
            else if (value == 4)
                endPoint = {x, y};

            ++x;
        }
        grid.push_back(row);
    }

    std::string texturePath;
    if (filename.find("maps1") != std::string::npos)
        texturePath = "assets/images/maps1.png";
    else if (filename.find("maps2") != std::string::npos)
        texturePath = "assets/images/maps2.png";

    if (!texturePath.empty())
        mapTexture.loadFromFile(texturePath);
    mapSprite.setTexture(mapTexture);

    return true;
}

void Map::draw(sf::RenderWindow& window) {
    window.draw(mapSprite);
}

bool Map::isPath(int x, int y) const {
    return grid[y][x] == 1;
}

bool Map::canPlaceTower(int x, int y) const {
    return grid[y][x] == 2;
}

const std::vector<sf::Vector2i>& Map::getSpawnPoints() const {
    return spawnPoints;
}

const std::vector<sf::Vector2i>& Map::getPath() const {
    return pathTiles;
}

sf::Vector2i Map::getEndPoint() const {
    return endPoint;
}
