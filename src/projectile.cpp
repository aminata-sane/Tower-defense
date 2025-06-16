#include "projectile.h"
#include "enemy.h"
#include <cmath>

static float getDistance(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Projectile::Projectile(ProjectileType type, sf::Vector2f startPos, Enemy *target, float damage)
    : position(startPos), target(target), speed(200.f), damage(damage), type(type), active(true)
{
    switch (type)
    {
    case ProjectileType::SWORD:
        texture.loadFromFile("assets/images/tower/knights.gif");
        hitBuffer.loadFromFile("assets/sounds/sword.wav");
        break;
    case ProjectileType::ARROW:
        texture.loadFromFile("assets/images/effect/arrow.gif");
        hitBuffer.loadFromFile("assets/sounds/arrows.wav");
        break;
    case ProjectileType::FIREBALL:
        texture.loadFromFile("assets/images/effect/fire.gif");
        hitBuffer.loadFromFile("assets/sounds/fire.wav");
        break;
    }

    hitSound.setBuffer(hitBuffer);

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(position);
}

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
        hitSound.play();
        active = false;
        return;
    }

    sf::Vector2f direction = toTarget / distance;
    position += direction * speed * deltaTime;
    sprite.setPosition(position);
}

void Projectile::draw(sf::RenderWindow &window)
{
    if (active)
        window.draw(sprite);
}

bool Projectile::isActive() const
{
    return active;
}

ProjectileType Projectile::getType() const
{
    return type;
}

sf::Vector2f Projectile::getPosition() const
{
    return position;
}
