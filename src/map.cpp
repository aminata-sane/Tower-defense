#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream> // Pour std::stringstream

Map::Map() : mapTileSize(64.f), mapWidthInTiles(0), mapHeightInTiles(0) { // Valeur par défaut pour tileSize
    // Charger les textures par défaut ou laisser vide jusqu'à loadFromFile
    if (!pathTexture.loadFromFile("assets/textures/path_tile.png")) { // REMPLACE AVEC TES CHEMINS
        std::cerr << "Erreur Map: Impossible de charger path_tile.png" << std::endl;
    }
    if (!buildableTexture.loadFromFile("assets/textures/buildable_tile.png")) { // REMPLACE
        std::cerr << "Erreur Map: Impossible de charger buildable_tile.png" << std::endl;
    }
    if (!obstacleTexture.loadFromFile("assets/textures/obstacle_tile.png")) { // REMPLACE
        std::cerr << "Erreur Map: Impossible de charger obstacle_tile.png" << std::endl;
    }
}

bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur Map: Impossible d'ouvrir le fichier de carte: " << filename << std::endl;
        return false;
    }

    tileData.clear();
    enemyPathWorldCoords.clear(); // Réinitialiser le chemin

    std::string line;
    int y = 0;
    // Lire d'abord les dimensions (supposons qu'elles sont sur la première ligne ou implicites)
    // Pour cet exemple, nous allons déduire les dimensions.
    
    // Lire la taille des tuiles (si présente dans le fichier, sinon utiliser la valeur par défaut)
    // Exemple: première ligne "TILESIZE 64"
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        ss >> key;
        if (key == "TILESIZE") {
            ss >> mapTileSize;
        } else {
            // Si la première ligne n'est pas TILESIZE, on la remet dans le stream ou on la traite comme une ligne de carte
            file.seekg(0); // Rembobine au début si la première ligne n'était pas TILESIZE
        }
    }


    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Ignorer les lignes vides ou les commentaires

        std::vector<int> row;
        std::stringstream ss(line);
        int tileType;
        while (ss >> tileType) {
            row.push_back(tileType);
        }
        if (!row.empty()) {
            tileData.push_back(row);
            if (mapWidthInTiles == 0) {
                mapWidthInTiles = row.size();
            } else if (row.size() != static_cast<size_t>(mapWidthInTiles)) {
                 std::cerr << "Erreur Map: Lignes de carte de longueurs incohérentes." << std::endl;
                 return false;
            }
        }
        y++;
    }
    mapHeightInTiles = tileData.size();

    if (mapWidthInTiles == 0 || mapHeightInTiles == 0) {
        std::cerr << "Erreur Map: Carte vide ou dimensions invalides." << std::endl;
        return false;
    }
    
    parsePathAndSpawn(); // Extraire le chemin et le point de spawn

    file.close();
    std::cout << "Carte chargée: " << filename << " (" << mapWidthInTiles << "x" << mapHeightInTiles << ")" << std::endl;
    return true;
}

void Map::parsePathAndSpawn() {
    enemyPathWorldCoords.clear();
    bool spawnFound = false;
    std::vector<sf::Vector2i> pathTileCoords; // Chemin en coordonnées de tuiles pour le tri

    // Trouver le point de spawn (type 3) et les tuiles de chemin (type 1)
    for (int y = 0; y < mapHeightInTiles; ++y) {
        for (int x = 0; x < mapWidthInTiles; ++x) {
            if (tileData[y][x] == 3) { // Point de spawn
                enemySpawnPointWorldCoords = sf::Vector2f(
                    static_cast<float>(x) * mapTileSize + mapTileSize / 2.0f,
                    static_cast<float>(y) * mapTileSize + mapTileSize / 2.0f
                );
                pathTileCoords.push_back({x,y}); // Le spawn est le début du chemin
                spawnFound = true;
            } else if (tileData[y][x] == 1) { // Tuile de chemin
                pathTileCoords.push_back({x,y});
            } else if (tileData[y][x] == 4) { // Tuile de fin (optionnel, mais utile pour ordonner le chemin)
                 pathTileCoords.push_back({x,y}); // Ajoute le point de fin pour aider à ordonner
            }
        }
    }

    if (!spawnFound) {
        std::cerr << "Avertissement Map: Aucun point de spawn (type 3) trouvé sur la carte." << std::endl;
        // Utiliser un spawn par défaut si nécessaire, ou considérer comme une erreur
        if (!pathTileCoords.empty()) { // Si on a des tuiles de chemin, prendre la première comme spawn
             enemySpawnPointWorldCoords = sf::Vector2f(
                static_cast<float>(pathTileCoords[0].x) * mapTileSize + mapTileSize / 2.0f,
                static_cast<float>(pathTileCoords[0].y) * mapTileSize + mapTileSize / 2.0f
            );
        } else {
            enemySpawnPointWorldCoords = sf::Vector2f(mapTileSize / 2.0f, mapTileSize / 2.0f); // Fallback très basique
        }
    }

    // Ordonner pathTileCoords pour former un chemin continu depuis le spawn.
    // C'est la partie la plus complexe. Un algorithme simple de type "voisin le plus proche"
    // ou une recherche de chemin (A*) serait nécessaire pour un chemin non trivial.
    // Pour un chemin simple et linéaire défini par l'ordre dans le fichier ou des indices,
    // cela pourrait être plus simple.
    // Ici, une version TRES simplifiée qui suppose que les tuiles de chemin sont adjacentes
    // et que le spawn est le premier point.
    if (!pathTileCoords.empty()) {
        std::vector<sf::Vector2i> sortedPathTiles;
        sf::Vector2i currentTile = {
            static_cast<int>(enemySpawnPointWorldCoords.x / mapTileSize),
            static_cast<int>(enemySpawnPointWorldCoords.y / mapTileSize)
        };
        
        // On ajoute le spawn en premier
        enemyPathWorldCoords.push_back(enemySpawnPointWorldCoords);
        
        // Retirer le spawn de la liste des tuiles à traiter si on l'a ajouté manuellement
        pathTileCoords.erase(std::remove_if(pathTileCoords.begin(), pathTileCoords.end(),
            [&](const sf::Vector2i& tile){ return tile.x == currentTile.x && tile.y == currentTile.y; }), pathTileCoords.end());


        // Boucle pour trouver les points suivants (simplification grossière)
        // Une vraie recherche de chemin est nécessaire pour des cartes complexes.
        // Cette logique est un placeholder et ne fonctionnera que pour des chemins très simples.
        while(!pathTileCoords.empty()){
            bool foundNext = false;
            for(size_t i=0; i < pathTileCoords.size(); ++i){
                sf::Vector2i nextPossible = pathTileCoords[i];
                int dx = std::abs(nextPossible.x - currentTile.x);
                int dy = std::abs(nextPossible.y - currentTile.y);
                if((dx == 1 && dy == 0) || (dx == 0 && dy == 1)){ // Adjacent
                    currentTile = nextPossible;
                    enemyPathWorldCoords.push_back(sf::Vector2f(
                        static_cast<float>(currentTile.x) * mapTileSize + mapTileSize / 2.0f,
                        static_cast<float>(currentTile.y) * mapTileSize + mapTileSize / 2.0f
                    ));
                    pathTileCoords.erase(pathTileCoords.begin() + i);
                    foundNext = true;
                    break;
                }
            }
            if(!foundNext) break; // Plus de tuile adjacente trouvée
        }
    }
    if (enemyPathWorldCoords.empty()) {
         std::cerr << "Avertissement Map: Impossible de déterminer le chemin des ennemis." << std::endl;
    }
}


void Map::draw(sf::RenderWindow& window) {
    sf::Sprite tileSprite; // Créer un sprite à réutiliser

    for (int y = 0; y < mapHeightInTiles; ++y) {
        for (int x = 0; x < mapWidthInTiles; ++x) {
            tileSprite.setPosition(static_cast<float>(x) * mapTileSize, static_cast<float>(y) * mapTileSize);
            int tileType = tileData[y][x];

            switch (tileType) {
                case 0: // Constructible
                    tileSprite.setTexture(buildableTexture);
                    break;
                case 1: // Chemin
                case 3: // Spawn (dessiné comme un chemin)
                case 4: // Fin (dessiné comme un chemin)
                    tileSprite.setTexture(pathTexture);
                    break;
                case 2: // Obstacle
                    tileSprite.setTexture(obstacleTexture);
                    break;
                default: // Inconnu, ne rien dessiner ou une texture par défaut
                    continue; 
            }
            // Assurer que la texture est valide avant de dessiner
            if (tileSprite.getTexture()) {
                 // Redimensionner le sprite à la taille de la tuile si les textures ne sont pas déjà à la bonne taille
                sf::Vector2u texSize = tileSprite.getTexture()->getSize();
                if (texSize.x > 0 && texSize.y > 0) {
                     tileSprite.setScale(mapTileSize / texSize.x, mapTileSize / texSize.y);
                }
                window.draw(tileSprite);
            }
        }
    }
}

sf::Vector2i Map::worldToTileCoords(sf::Vector2f worldPos) const {
    if (mapTileSize <= 0) return {-1, -1}; // Éviter la division par zéro
    return {
        static_cast<int>(worldPos.x / mapTileSize),
        static_cast<int>(worldPos.y / mapTileSize)
    };
}

bool Map::isTileValid(int tileX, int tileY) const {
    return tileX >= 0 && tileX < mapWidthInTiles &&
           tileY >= 0 && tileY < mapHeightInTiles;
}

float Map::getTileSize() const {
    return mapTileSize;
}

bool Map::isPath(int tileX, int tileY) const {
    if (!isTileValid(tileX, tileY)) return false;
    // 1: chemin, 3: spawn (est un chemin), 4: fin (est un chemin)
    return tileData[tileY][tileX] == 1 || tileData[tileY][tileX] == 3 || tileData[tileY][tileX] == 4;
}

bool Map::isTileBuildable(int tileX, int tileY) const {
    if (!isTileValid(tileX, tileY)) return false;
    return tileData[tileY][tileX] == 0; // '0' est le code pour une tuile constructible
}

const std::vector<sf::Vector2f>& Map::getEnemyPath() const {
    return enemyPathWorldCoords;
}

sf::Vector2f Map::getEnemySpawnPoint() const {
    return enemySpawnPointWorldCoords;
}
