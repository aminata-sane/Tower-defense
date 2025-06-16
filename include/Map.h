#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
public:
    Map();
    bool loadFromFile(const std::string& filename); // Charge la disposition de la carte et les textures/sprites
    void draw(sf::RenderWindow& window);    // Dessine la carte tuile par tuile

    // Méthodes utilitaires
    sf::Vector2i worldToTileCoords(sf::Vector2f worldPos) const;
    bool isTileValid(int tileX, int tileY) const; // Vérifie si les coordonnées de tuile sont dans les limites
    float getTileSize() const;

    // Méthodes pour la logique de jeu (chemin, placement)
    bool isPath(int tileX, int tileY) const;            // AJOUTÉ : Vérifie si une tuile est un chemin
    bool isTileBuildable(int tileX, int tileY) const; // AJOUTÉ : Vérifie si on peut construire sur une tuile
    
    const std::vector<sf::Vector2f>& getEnemyPath() const; // Retourne le chemin des ennemis (en coordonnées du monde)
    sf::Vector2f getEnemySpawnPoint() const; // Retourne le point de spawn (en coordonnées du monde)


private:
    std::vector<std::vector<int>> tileData; // 0: constructible, 1: chemin, 2: obstacle, 3: spawn, 4: end
    
    // Ressources pour le rendu des tuiles
    sf::Texture pathTexture;
    sf::Texture buildableTexture;
    sf::Texture obstacleTexture;
    // Tu peux ajouter d'autres textures pour spawn, end, etc., ou utiliser les mêmes.
    // Ou un seul tileset et utiliser sf::IntRect pour sélectionner la bonne tuile.
    // Pour la simplicité ici, des textures séparées sont supposées.

    float mapTileSize;      // Taille d'une tuile en pixels
    int mapWidthInTiles;    // Largeur de la carte en nombre de tuiles
    int mapHeightInTiles;   // Hauteur de la carte en nombre de tuiles

    std::vector<sf::Vector2f> enemyPathWorldCoords; // Chemin des ennemis (coordonnées du monde, centres des tuiles)
    sf::Vector2f enemySpawnPointWorldCoords; // Point de spawn (coordonnées du monde)
    // sf::Vector2f enemyEndPointWorldCoords; // Si tu as besoin du point de fin explicitement

    void parsePathAndSpawn(); // Méthode privée pour extraire le chemin et le spawn depuis tileData
};

#endif // MAP_H
