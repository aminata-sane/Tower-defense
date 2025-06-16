#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Enemy
{
public:
    virtual ~Enemy() = default;

    void update(float dt);
    void draw(sf::RenderWindow &window);
    void takeDamage(int amount);

    bool isDead() const;
    bool reachedGoal() const;

    bool isAlive() const;
    int getHealth() const;

        sf::Vector2f getPosition() const;

    int getHp() const;
    virtual int getMaxHp() const = 0;

protected:
    Enemy(const std::vector<sf::Vector2f> &path, float speed, int hp, const std::string &texturePath);

    std::vector<sf::Vector2f> path;
    size_t currentTarget;
    sf::Vector2f position;
    float speed;
    int hp;
    int maxHp;

    sf::Texture texture;
    sf::Sprite sprite;

    void moveTowardsTarget(float dt);
};

#endif