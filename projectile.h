// #ifndef PROJECTILE_H
// #define PROJECTILE_H

// #include <SFML/Graphics.hpp>
// #include <memory>

// class Enemy; // Forward declaration

// // Enum for projectile types
// enum class ProjectileType
// {
//     SWORD,   // Close-range melee projectile
//     ARROW,   // Ranged physical projectile
//     FIREBALL // Magic explosive projectile
// };

// class Projectile
// {
// public:
//     // Constructor
//     Projectile(sf::Vector2f startPos,
//                std::shared_ptr<Enemy> target,
//                float speed,
//                int damage,
//                ProjectileType type);

//     // Update projectile logic
//     void update(float deltaTime);

//     // Draw projectile to the screen
//     void draw(sf::RenderWindow &window);

//     // Check if projectile is still active
//     bool isActive() const;

//     // Get projectile type
//     ProjectileType getType() const;

// private:
//     sf::Vector2f position;         // Current position
//     std::shared_ptr<Enemy> target; // Target enemy
//     float speed;                   // Movement speed
//     int damage;                    // Damage to deal
//     bool active;                   // Is the projectile still active?
//     ProjectileType type;           // Type of projectile

//     sf::Texture texture; // Texture for sprite
//     sf::Sprite sprite;   // Drawable sprite
// };

// #endif // PROJECTILE_H

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Enemy; // Forward declaration

// Enum for projectile types
enum class ProjectileType
{
    SWORD,   // Close-range melee projectile
    ARROW,   // Ranged physical projectile
    FIREBALL // Magic explosive projectile
};

class Projectile
{
public:
    // Constructor
    Projectile(ProjectileType type,
               sf::Vector2f startPos,
               std::shared_ptr<Enemy> target,
               float damage);

    // Update projectile logic
    void update(float deltaTime);

    // Draw projectile to the screen
    void draw(sf::RenderWindow &window);

    // Check if projectile is still active
    bool isActive() const;

    // Get projectile type
    ProjectileType getType() const;

    // Get projectile position (for effects or debug)
    sf::Vector2f getPosition() const;

private:
    sf::Vector2f position;         // Current position
    std::shared_ptr<Enemy> target; // Target enemy
    float speed;                   // Movement speed
    float damage;                  // Damage to deal
    bool active;                   // Is the projectile still active?
    ProjectileType type;           // Type of projectile

    sf::Texture texture; // Texture for sprite
    sf::Sprite sprite;   // Drawable sprite

    sf::SoundBuffer hitBuffer; // Sound for hit
    sf::Sound hitSound;        // Sound object
};

#endif // PROJECTILE_H
