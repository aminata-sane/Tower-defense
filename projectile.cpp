// #include "projectile.h"
// #include "enemy.h"
// #include <cmath>

// // Helper to calculate distance
// static float getDistance(sf::Vector2f a, sf::Vector2f b)
// {
//     return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
// }

// // Base Projectile constructor
// Projectile::Projectile(sf::Vector2f startPos, std::shared_ptr<Enemy> target, float speed, int damage, ProjectileType type)
//     : position(startPos), target(target), speed(speed), damage(damage), type(type), active(true)
// {

//     // Load texture depending on type
//     switch (type)
//     {
//     case ProjectileType::SWORD:
//         texture.loadFromFile("assets/images/sword.png");
//         break;
//     case ProjectileType::ARROW:
//         texture.loadFromFile("assets/images/arrow.png");
//         break;
//     case ProjectileType::FIREBALL:
//         texture.loadFromFile("assets/images/fireball.png");
//         break;
//     }

//     sprite.setTexture(texture);
//     sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
//     sprite.setPosition(position);
// }

// // Update projectile each frame
// void Projectile::update(float deltaTime)
// {
//     if (!target || !target->isAlive())
//     {
//         active = false;
//         return;
//     }

//     sf::Vector2f toTarget = target->getPosition() - position;
//     float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

//     if (distance < 5.f)
//     {
//         target->takeDamage(damage);
//         active = false;
//         return;
//     }

//     sf::Vector2f direction = toTarget / distance;
//     position += direction * speed * deltaTime;
//     sprite.setPosition(position);
// }

// // Draw projectile
// void Projectile::draw(sf::RenderWindow &window)
// {
//     if (active)
//         window.draw(sprite);
// }

// // Check if projectile is active
// bool Projectile::isActive() const
// {
//     return active;
// }

// // Get projectile type
// ProjectileType Projectile::getType() const
// {
//     return type;
// }

#include "projectile.h"
#include "enemy.h"
#include <cmath>

// Helper to calculate distance
static float getDistance(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Projectile constructor
Projectile::Projectile(ProjectileType type,
                       sf::Vector2f startPos,
                       std::shared_ptr<Enemy> target,
                       float damage)
    : position(startPos), target(target), speed(200.f), damage(damage), type(type), active(true)
{
    // Load texture depending on projectile type
    switch (type)
    {
    case ProjectileType::SWORD:
        texture.loadFromFile("assets/images/sword.png");
        hitBuffer.loadFromFile("assets/sounds/sword_hit.wav");
        break;
    case ProjectileType::ARROW:
        texture.loadFromFile("assets/images/arrow.png");
        hitBuffer.loadFromFile("assets/sounds/arrow_hit.wav");
        break;
    case ProjectileType::FIREBALL:
        texture.loadFromFile("assets/images/fireball.png");
        hitBuffer.loadFromFile("assets/sounds/fireball_hit.wav");
        break;
    }

    hitSound.setBuffer(hitBuffer);

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(position);
}

// Update projectile each frame
void Projectile::update(float deltaTime)
{
    if (!target || !target->isAlive())
    {
        active = false;
        return;
    }

    sf::Vector2f toTarget = target->getPosition() - position;
    float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    if (distance < 5.f)
    {
        target->takeDamage(damage);
        hitSound.play(); // Play hit sound
        active = false;
        return;
    }

    sf::Vector2f direction = toTarget / distance;
    position += direction * speed * deltaTime;
    sprite.setPosition(position);
}

// Draw projectile
void Projectile::draw(sf::RenderWindow &window)
{
    if (active)
        window.draw(sprite);
}

// Check if projectile is active
bool Projectile::isActive() const
{
    return active;
}

// Get projectile type
ProjectileType Projectile::getType() const
{
    return type;
}

// Get projectile position (optional utility)
sf::Vector2f Projectile::getPosition() const
{
    return position;
}
