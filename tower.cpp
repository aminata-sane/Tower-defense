#include "tower.h"
#include "enemy.h"
#include "projectile.h"

#include <cmath>
#include <iostream>

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
        texture.loadFromFile("assets/images/tower_melee.png");
        break;
    case TowerType::ARCHER:
        range = 150.f;
        fireRate = 0.8f;
        cost = 75;
        upgradeCost = 50;
        damage = 15;
        texture.loadFromFile("assets/images/tower_archer.png");
        break;
    case TowerType::MAGIC:
        range = 120.f;
        fireRate = 0.6f;
        cost = 100;
        upgradeCost = 70;
        damage = 25;
        texture.loadFromFile("assets/images/tower_magic.png");
        break;
    }

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

// Upgrade tower
bool Tower::upgrade(int &playerGold)
{
    if (playerGold >= upgradeCost)
    {
        playerGold -= upgradeCost;
        level++;
        range *= 1.1f;
        fireRate *= 0.9f; // faster
        damage += 10;
        upgradeCost += 25;
        return true;
    }
    return false;
}

// Return true if enemy is in range
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
void Tower::update(float deltaTime, std::vector<std::shared_ptr<Enemy>> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles)
{
    timeSinceLastShot += deltaTime;

    // Acquire new target if no target or target is dead/out of range
    if (!target || !target->isAlive() || !isInRange(target))
    {
        target = acquireTarget(enemies);
    }

    // Shoot if possible
    if (target && timeSinceLastShot >= fireRate)
    {
        shoot(projectiles);
        timeSinceLastShot = 0.f;
    }
}

// Shoot a projectile
void Tower::shoot(std::vector<std::shared_ptr<Projectile>> &projectiles)
{
    if (!target)
        return;

    ProjectileType projectileType;

    switch (type)
    {
    case TowerType::MELEE:
        projectileType = ProjectileType::SWORD;
        break;
    case TowerType::ARCHER:
        projectileType = ProjectileType::ARROW;
        break;
    case TowerType::MAGIC:
        projectileType = ProjectileType::FIREBALL;
        break;
    }

    // Create and push new projectile
    projectiles.push_back(std::make_shared<Projectile>(projectileType, position, target, damage));
}

// Draw the tower
void Tower::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

// Get tower position
sf::Vector2f Tower::getPosition() const
{
    return position;
}
