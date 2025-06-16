#include "enemy.h"
#include <iostream> // Pour les messages d'erreur
#include <cmath>    // Pour std::sqrt, std::atan2

// Constructeur
Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, int initialHealth, const std::string& texturePath)
    : pathPoints(path),      // Initialise la référence au chemin
      moveSpeed(speed),
      maxHealthPoints(initialHealth > 0 ? initialHealth : 1), // Assure que maxHealth est au moins 1
      currentHealthPoints(maxHealthPoints),
      currentPathTargetIndex(0),
      hasReachedGoal(false) {

    if (!pathPoints.empty()) {
        currentPosition = pathPoints[0]; // Initialise la position au premier point du chemin
    } else {
        currentPosition = sf::Vector2f(0, 0); // Position par défaut si pas de chemin
        std::cerr << "Avertissement Enemy: Chemin vide fourni." << std::endl;
    }

    if (!enemyTexture.loadFromFile(texturePath)) {
        std::cerr << "Erreur Enemy: Impossible de charger la texture depuis " << texturePath << std::endl;
        // Peut-être charger une texture par défaut ou laisser le sprite vide
    }
    sprite.setTexture(enemyTexture);

    if (enemyTexture.getSize().x > 0 && enemyTexture.getSize().y > 0) {
        sprite.setOrigin(enemyTexture.getSize().x / 2.f, enemyTexture.getSize().y / 2.f);
    }
    sprite.setPosition(currentPosition);
}

void Enemy::update(float dt) {
    if (isDead() || hasReachedGoal) {
        return; // Ne rien faire si mort ou a atteint le but
    }

    if (currentPathTargetIndex < pathPoints.size()) {
        moveTowards(pathPoints[currentPathTargetIndex], dt);
    }
    sprite.setPosition(currentPosition); // Met à jour la position du sprite
}

// Méthode helper pour le mouvement
void Enemy::moveTowards(sf::Vector2f target, float dt) {
    sf::Vector2f direction = target - currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= moveSpeed * dt) { // Si on peut atteindre ou dépasser la cible ce frame
        currentPosition = target;
        currentPathTargetIndex++;
        if (currentPathTargetIndex >= pathPoints.size()) {
            hasReachedGoal = true;
            // std::cout << "Enemy reached goal!" << std::endl; // Debug
        }
    } else {
        direction /= distance; // Normaliser le vecteur direction
        currentPosition += direction * moveSpeed * dt;
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isDead()) { // Ne dessine que si vivant
        window.draw(sprite);
    }
}

bool Enemy::isDead() const {
    return currentHealthPoints <= 0;
}

void Enemy::takeDamage(int amount) {
    if (amount > 0) {
        currentHealthPoints -= amount;
        if (currentHealthPoints < 0) {
            currentHealthPoints = 0;
        }
        // std::cout << "Enemy took " << amount << " damage, HP: " << currentHealthPoints << std::endl; // Debug
    }
}

bool Enemy::reachedGoal() const {
    return hasReachedGoal;
}

sf::Vector2f Enemy::getPosition() const {
    return currentPosition; // Ou sprite.getPosition() si c'est la source de vérité
}

int Enemy::getCurrentHealth() const {
    return currentHealthPoints;
}

int Enemy::getMaxHp() const {
    return maxHealthPoints; // Assure que maxHealthPoints est bien un membre de Enemy
}

// Implémentations des méthodes virtuelles ajoutées (valeurs par défaut, à surcharger dans les classes dérivées)
int Enemy::getDamageToPlayer() const {
    return 1; // Dégât par défaut si l'ennemi atteint le but
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

int Enemy::getGoldValue() const {
    return 10; // Valeur en or par défaut
}
