#include "enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp, const std::string& texturePath)
    : path(path), speed(speed), hp(hp), maxHp(hp), currentTarget(0) {
    if (!path.empty()) {
        position = path[0];
    }
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

void Enemy::update(float dt) {
    if (currentTarget < path.size()) {
        moveTowardsTarget(dt);
    }
    sprite.setPosition(position);
}

void Enemy::moveTowardsTarget(float dt) {
    sf::Vector2f target = path[currentTarget];
    sf::Vector2f direction = target - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.f) {
        ++currentTarget;
    } else {
        direction /= distance;
        position += direction * speed * dt;
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    sf::RectangleShape bg(sf::Vector2f(40, 5));
    bg.setFillColor(sf::Color::Red);
    bg.setPosition(position.x - 20, position.y - 40);

    sf::RectangleShape bar(sf::Vector2f(40.f * hp / maxHp, 5));
    bar.setFillColor(sf::Color::Green);
    bar.setPosition(position.x - 20, position.y - 40);

    window.draw(bg);
    window.draw(bar);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
}

bool Enemy::isDead() const {
    return hp <= 0;
}

bool Enemy::reachedGoal() const {
    return currentTarget >= path.size();
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

int Enemy::getHp() const {
    return hp;
}
