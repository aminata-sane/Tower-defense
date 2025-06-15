#include "tower.h"
#include "enemy.h"
#include "projectile.h"

#include <cmath>
#include <iostream>
#include <random>

// Constructor
Tower::Tower(TowerType type, sf::Vector2f position)
    : type(type), position(position), target(nullptr), timeSinceLastShot(0.f), level(1)
{
    switch (type)
    {
    case TowerType::MELEE:
        range = 80.f;
        fireRate = 1.0f;
        cost = 50;
        upgradeCost = 30;
        damage = 20;
        soundBuffer.loadFromFile("assets/sounds/sword.wav");
        break;
    case TowerType::ARROW:
        range = 150.f;
        fireRate = 0.8f;
        cost = 75;
        upgradeCost = 50;
        damage = 15;
        soundBuffer.loadFromFile("assets/sounds/arrows.wav");
        break;
    case TowerType::FIRE:
        range = 120.f;
        fireRate = 0.6f;
        cost = 100;
        upgradeCost = 70;
        damage = 25;
        soundBuffer.loadFromFile("assets/sounds/fire.wav");
        break;
    }

    shootSound.setBuffer(soundBuffer);

    // Load upgrade sound
    if (!upgradeBuffer.loadFromFile("assets/sounds/upgrade.wav"))
        std::cerr << "Failed to load upgrade.wav\n";
    upgradeSound.setBuffer(upgradeBuffer);

    // Load sprite
    updateTexture(); // Load texture based on type and level

    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

// Get tower type
TowerType Tower::getType() const
{
    return type;
}

// Get tower cost
int Tower::getCost() const
{
    return cost;
}

// Get tower upgrade cost
int Tower::getUpgradeCost() const
{
    return upgradeCost;
}

// Get tower level
int Tower::getLevel() const
{
    return level;
}

// Get tower position
sf::Vector2f Tower::getPosition() const
{
    return position;
}

// Upgrade tower
bool Tower::upgrade(int &playerGold)
{
    if (playerGold >= upgradeCost)
    {
        playerGold -= upgradeCost;
        level++;
        range *= 1.1f;
        fireRate *= 0.9f; // faster firing
        damage += 10;
        upgradeCost += 25;

        updateTexture();     // Refresh sprite
        upgradeSound.play(); // Play upgrade sound
        return true;
    }
    return false;
}

// Check if enemy is in range
bool Tower::isInRange(std::shared_ptr<Enemy> enemy) const
{
    float dx = enemy->getPosition().x - position.x;
    float dy = enemy->getPosition().y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

// Acquire target (nearest enemy within range)
std::shared_ptr<Enemy> Tower::acquireTarget(const std::vector<std::shared_ptr<Enemy>> &enemies)
{
    float closestDistance = range;
    std::shared_ptr<Enemy> closest = nullptr;

    for (auto &enemy : enemies)
    {
        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= closestDistance && enemy->isAlive())
        {
            closestDistance = distance;
            closest = enemy;
        }
    }
    return closest;
}

// Update tower logic (cooldown, targeting, shooting)
void Tower::update(float deltaTime,
                   std::vector<std::shared_ptr<Enemy>> &enemies,
                   std::vector<std::shared_ptr<Projectile>> &projectiles,
                   int &playerGold)
{
    timeSinceLastShot += deltaTime;

    // Acquire new target if needed
    if (!target || !target->isAlive() || !isInRange(target))
    {
        target = acquireTarget(enemies);
    }

    // Shoot if possible
    if (target && timeSinceLastShot >= fireRate)
    {
        auto newProjectile = shoot();
        if (newProjectile)
            projectiles.push_back(newProjectile);

        timeSinceLastShot = 0.f;
    }
}

// Shoot a projectile
std::shared_ptr<Projectile> Tower::shoot()
{
    if (!target)
        return nullptr;

    ProjectileType projectileType;

    switch (type)
    {
    case TowerType::MELEE:
        projectileType = ProjectileType::SWORD;
        break;
    case TowerType::ARROW:
        projectileType = ProjectileType::ARROW;
        break;
    case TowerType::FIRE:
        projectileType = ProjectileType::FIREBALL;
        break;
    }

    shootSound.play();

    return std::make_shared<Projectile>(projectileType, position, target, damage);
}

// Draw the tower
void Tower::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

// Update tower sprite based on type and level
void Tower::updateTexture()
{
    std::string basePath = "assets/images/tower/";
    std::string filename;

    switch (type)
    {
    case TowerType::MELEE:
        // Files: tower1-1.jpg, tower1-2.jpg, tower1-3.jpg
        filename = "tower1-" + std::to_string(level) + ".jpg";
        break;
    case TowerType::ARROW:
        // Files: archer-tower1-1.jpg, archer-tower1-2.jpg, archer-tower1-3.jpg
        filename = "archer-tower1-" + std::to_string(level) + ".jpg";
        break;
    case TowerType::FIRE:
        // Files: fire-tower1-1.jpg, fire-tower1-2.jpg, fire-tower1-3.jpg
        filename = "fire-tower1-" + std::to_string(level) + ".jpg";
        break;
    }

    std::string fullPath = basePath + filename;

    if (texture.loadFromFile(fullPath))
    {
        sprite.setTexture(texture);
    }
    else
    {
        std::cerr << "Failed to load texture: " << fullPath << std::endl;
    }
}
