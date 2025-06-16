#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Enemy; // Forward declaration to avoid circular dependency

enum class ProjectileType
{
    SWORD,
    ARROW,
    FIREBALL
};

class Projectile
{
public:
    Projectile(ProjectileType type, sf::Vector2f startPos, Enemy *target, float damage);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window);

    bool isActive() const;
    ProjectileType getType() const;
    sf::Vector2f getPosition() const;

private:
    sf::Vector2f position;
    Enemy *target;
    float speed;
    float damage;
    ProjectileType type;
    bool active;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::SoundBuffer hitBuffer;
    sf::Sound hitSound;
};

#endif // PROJECTILE_H
