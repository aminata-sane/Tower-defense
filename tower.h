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
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

// Forward declarations
class Enemy;
class Projectile;

// Enum to define tower types
enum class TowerType
{
    MELEE,
    ARROW,
    FIRE
};

class Tower
{
public:
    // Constructor to initialize the tower
    Tower(TowerType type, sf::Vector2f position);

    // Getter for tower type
    TowerType getType() const;

    // Getter for tower construction cost
    int getCost() const;

    // Getter for tower upgrade cost
    int getUpgradeCost() const;

    // Getter for tower level
    int getLevel() const;

    // Getter for tower position
    sf::Vector2f getPosition() const;

    // Upgrade tower (returns true if upgraded successfully)
    bool upgrade(int &playerGold);

    // Update tower logic (timing, targeting, shooting)
    void update(float deltaTime,
                std::vector<std::shared_ptr<Enemy>> &enemies,
                std::vector<std::shared_ptr<Projectile>> &projectiles,
                int &playerGold);

    // Draw the tower sprite on the window
    void draw(sf::RenderWindow &window);

private:
    TowerType type;          // Tower type (Melee, Arrow, Fire)
    sf::Vector2f position;   // Position of the tower
    sf::Texture texture;     // Texture used for rendering
    sf::Sprite sprite;       // Sprite for rendering
    float range;             // Attack range
    float fireRate;          // Rate of fire (in seconds)
    float timeSinceLastShot; // Time since last shot
    int cost;                // Cost to build
    int upgradeCost;         // Cost to upgrade
    int damage;              // Damage per projectile
    int level;               // Upgrade level

    sf::SoundBuffer soundBuffer; // Sound for shooting
    sf::Sound shootSound;        // Sound object
    sf::SoundBuffer upgradeBuffer;
    sf::Sound upgradeSound;

    std::shared_ptr<Enemy> target; // Currently acquired target

    // Find the closest enemy within range
    std::shared_ptr<Enemy> acquireTarget(const std::vector<std::shared_ptr<Enemy>> &enemies);

    // Check if enemy is within tower's range
    bool isInRange(std::shared_ptr<Enemy> enemy) const;

    // Shoot at the acquired target
    std::shared_ptr<Projectile> shoot();

    // Update sprite texture based on type and level
    void updateTexture();
};

#endif // TOWER_H
