// #ifndef TOWER_H
// #define TOWER_H

// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <memory>

// // Forward declarations
// class Enemy;
// class Projectile;

// // Enum to define tower types
// enum class TowerType
// {
//     MELEE,
//     ARCHER,
//     MAGIC
// };

// class Tower
// {
// public:
//     // Constructor to initialize the tower
//     Tower(TowerType type, sf::Vector2f position);

//     // Getter for tower type
//     TowerType getType() const;

//     // Getter for tower construction cost
//     int getCost() const;

//     // Getter for tower upgrade cost
//     int getUpgradeCost() const;

//     // Getter for tower level
//     int getLevel() const;

//     // Getter for tower position
//     sf::Vector2f getPosition() const;

//     // Upgrade tower (returns true if upgraded successfully)
//     bool upgrade(int &playerGold);

//     // Update tower logic (timing, targeting, shooting)
//     void update(float deltaTime,
//                 std::vector<std::shared_ptr<Enemy>> &enemies,
//                 std::vector<std::shared_ptr<Projectile>> &projectiles);

//     // Draw the tower sprite on the window
//     void draw(sf::RenderWindow &window);

// private:
//     TowerType type;          // Tower type (Melee, Archer, Magic)
//     sf::Vector2f position;   // Position of the tower
//     sf::Texture texture;     // Texture used for rendering
//     sf::Sprite sprite;       // Sprite for rendering
//     float range;             // Attack range
//     float fireRate;          // Rate of fire (in seconds)
//     float timeSinceLastShot; // Time since last shot
//     int cost;                // Cost to build
//     int upgradeCost;         // Cost to upgrade
//     int damage;              // Damage per projectile
//     int level;               // Upgrade level

//     std::shared_ptr<Enemy> target; // Currently acquired target

//     // Find the closest enemy within range
//     std::shared_ptr<Enemy> acquireTarget(const std::vector<std::shared_ptr<Enemy>> &enemies);

//     // Check if enemy is within tower's range
//     bool isInRange(std::shared_ptr<Enemy> enemy) const;

//     // Shoot at the acquired target
//     void shoot(std::vector<std::shared_ptr<Projectile>> &projectiles);
// };

// #endif // TOWER_H
#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Pour les sons
#include <vector>
#include <memory> // Pour std::unique_ptr et std::shared_ptr
#include <string>

class Enemy;      // Déclaration anticipée
class Projectile; // Déclaration anticipée
class Player;     // Déclaration anticipée

// Énumération pour les types de tours (cohérente avec tower.cpp)
enum class TowerType {
    ARCHER_TOWER, // Renommé pour éviter la confusion avec ProjectileType::ARROW
    CANNON_TOWER, // Renommé
    MAGE_TOWER,   // Nouveau type pour correspondre à FIRE/MAGE
    MELEE_POINT,  // Si c'est un point de défense de mêlée
    UNKNOWN
};

class Tower {
public:
    // Constructeur mis à jour pour prendre le type et la position.
    // Les autres stats (range, fireRate, damage, texture) seront définies dans le constructeur en fonction du type.
    Tower(TowerType type, sf::Vector2f position);
    virtual ~Tower() = default;

    virtual void update(float dt,
                        const std::vector<std::shared_ptr<Enemy>>& all_enemies, // NOUVEAU
                        std::vector<std::unique_ptr<Projectile>>& game_projectiles,
                        Player& game_player); // Player peut être nécessaire pour les coûts de tir spéciaux

    virtual void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    TowerType getTowerType() const; // Renommé pour clarté
    int getLevel() const;
    int getUpgradeCost() const;
    bool canUpgrade(int playerGold) const;
    virtual void upgrade(); // Méthode pour améliorer la tour

    static int getPlacementCost(TowerType type); // Coût statique pour placer une tour

protected:
    sf::Sprite sprite;
    sf::Texture towerTexture;

    TowerType currentType;
    sf::Vector2f towerPosition; // Renommé pour clarté
    
    // Statistiques de la tour
    float attackRange;
    float shotsPerSecond; // Renommé depuis fireRate pour plus de clarté
    float timeUntilNextShot; // Renommé depuis fireCooldown
    int attackDamage;
    
    int currentLevel;
    int maxLevel;
    int initialCost;        // Coût pour placer la tour initialement
    int costToUpgrade;      // Coût pour la prochaine amélioration

    std::shared_ptr<Enemy> currentTarget; // Revenir à shared_ptr si la liste principale est shared_ptr

    // Sons
    sf::SoundBuffer shootSoundBuffer;
    sf::Sound towerShootSound;
    sf::SoundBuffer upgradeSoundBuffer;
    sf::Sound towerUpgradeSound;

    // Méthodes helper protégées
    virtual void shootAt(std::shared_ptr<Enemy> target, std::vector<std::unique_ptr<Projectile>>& game_projectiles); // Si shared_ptr
    void findNewTarget(const std::vector<std::shared_ptr<Enemy>>& all_enemies); // NOUVEAU
    void loadResourcesBasedOnType(); // Pour charger texture, sons, et stats initiales
    virtual void applyUpgradeStats();  // Appliquer les changements de stats lors de l'amélioration
};

#endif // TOWER_H
