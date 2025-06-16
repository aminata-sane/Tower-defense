#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "projectile.h"

class Enemy;

enum class TowerType
{
    MELEE,
    ARROW,
    FIRE
};

class Tower
{
public:
    Tower(TowerType type, sf::Vector2f position);

    void update(float deltaTime,
                std::vector<Enemy *> &enemies,
                std::vector<std::shared_ptr<Projectile>> &projectiles,
                int &playerGold);

    void draw(sf::RenderWindow &window);
    bool upgrade(int &playerGold);

    TowerType getType() const;
    int getCost() const;
    int getUpgradeCost() const;
    int getLevel() const;
    sf::Vector2f getPosition() const;

private:
    TowerType type;
    sf::Vector2f position;
    float range;
    float fireRate;
    float timeSinceLastShot;
    int cost;
    int upgradeCost;
    int level;
    float damage;

    Enemy *target;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::SoundBuffer soundBuffer;
    sf::Sound shootSound;

    sf::SoundBuffer upgradeBuffer;
    sf::Sound upgradeSound;

    void updateTexture();
    Enemy *acquireTarget(const std::vector<Enemy *> &enemies);
    bool isInRange(Enemy *enemy) const;
    std::shared_ptr<Projectile> shoot();
};

#endif // TOWER_H
