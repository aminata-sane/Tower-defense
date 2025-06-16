#include "player.h"

// Constructeur
Player::Player(int initialHealth, int initialGold)
    : health(initialHealth > 0 ? initialHealth : 1), // Assure une santé initiale positive
      gold(initialGold >= 0 ? initialGold : 0),     // Assure un or initial non négatif
      score(0) {}

// Getters
int Player::getHealth() const {
    return health;
}

int Player::getGold() const {
    return gold;
}

int Player::getScore() const {
    return score;
}

// Actions
void Player::takeDamage(int amount) {
    if (amount > 0) { // Ne prend en compte que les dégâts positifs
        health -= amount;
        if (health < 0) {
            health = 0; // Empêche la santé de devenir négative
        }
    }
}

void Player::addGold(int amount) {
    if (amount > 0) { // N'ajoute que des montants d'or positifs
        gold += amount;
    }
}

bool Player::spendGold(int amount) {
    if (amount > 0 && gold >= amount) { // Ne dépense que des montants positifs et si l'or est suffisant
        gold -= amount;
        return true;
    }
    return false;
}

void Player::addScore(int points) {
    if (points > 0) { // N'ajoute que des points positifs
        score += points;
    }
    // Si des points négatifs sont possibles pour une pénalité, cette condition devrait être ajustée.
}

bool Player::isAlive() const {
    return health > 0;
}
