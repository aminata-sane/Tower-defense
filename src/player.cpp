#include "player.h"

// Constructeur
Player::Player(int health, int gold)
    : health(health), gold(gold), score(0) {}

// Getters
int Player::getHealth() const
{
    return health;
}

int Player::getGold() const
{
    return gold;
}

int Player::getScore() const
{
    return score;
}

// Actions
void Player::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
        health = 0;
}

void Player::addGold(int amount)
{
    gold += amount;
}

bool Player::spendGold(int amount)
{
    if (gold >= amount)
    {
        gold -= amount;
        return true;
    }
    return false;
}

void Player::addScore(int points)
{
    score += points;
}

bool Player::isAlive() const
{
    return health > 0;
}