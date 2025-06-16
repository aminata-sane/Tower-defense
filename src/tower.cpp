#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include <cmath>
#include <iostream>
#include <random>

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

    upgradeBuffer.loadFromFile("assets/sounds/upgrade.wav");
    upgradeSound.setBuffer(upgradeBuffer);

    updateTexture();
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

TowerType Tower::getType() const { return type; }
int Tower::getCost() const { return cost; }
int Tower::getUpgradeCost() const { return upgradeCost; }
int Tower::getLevel() const { return level; }
sf::Vector2f Tower::getPosition() const { return position; }

bool Tower::upgrade(int &playerGold)
{
    if (playerGold >= upgradeCost)
    {
        playerGold -= upgradeCost;
        level++;
        range *= 1.1f;
        fireRate *= 0.9f;
        damage += 10;
        upgradeCost += 25;
        updateTexture();
        upgradeSound.play();
        return true;
    }
    return false;
}

bool Tower::isInRange(Enemy *enemy) const
{
    float dx = enemy->getPosition().x - position.x;
    float dy = enemy->getPosition().y - position.y;
    return std::sqrt(dx * dx + dy * dy) <= range;
}

Enemy *Tower::acquireTarget(const std::vector<Enemy *> &enemies)
{
    float closestDistance = range;
    Enemy *closest = nullptr;

    for (Enemy *enemy : enemies)
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

void Tower::update(float deltaTime, std::vector<Enemy *> &enemies, std::vector<std::shared_ptr<Projectile>> &projectiles, int &playerGold)
{
    timeSinceLastShot += deltaTime;
    if (!target || !target->isAlive() || !isInRange(target))
    {
        target = acquireTarget(enemies);
    }
    if (target && timeSinceLastShot >= fireRate)
    {
        auto newProjectile = shoot();
        if (newProjectile)
            projectiles.push_back(newProjectile);
        timeSinceLastShot = 0.f;
    }
}

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

void Tower::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Tower::updateTexture()
{
    std::string basePath = "assets/images/tower/";
    std::string filename;

    switch (type)
    {
    case TowerType::MELEE:
        filename = "tower1-" + std::to_string(level) + ".jpg";
        break;
    case TowerType::ARROW:
        filename = "archer-tower1-" + std::to_string(level) + ".jpg";
        break;
    case TowerType::FIRE:
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