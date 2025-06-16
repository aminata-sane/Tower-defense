#include "projectile.h"
#include "enemy.h" // Si on utilise std::shared_ptr<Enemy>
#include <iostream>
#include <cmath> // Pour std::atan2, std::cos, std::sin, std::sqrt

// Helper to calculate distance
static float getDistance(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Projectile constructor
Projectile::Projectile(ProjectileType type,
                       sf::Vector2f startPosition,
                       sf::Vector2f targetPosition,
                       float speed,
                       int damage)
    : projType(type),
      currentPosition(startPosition),
      moveSpeed(speed),
      damageAmount(damage),
      activeState(true) {

    loadResources(); // Charge la texture (et le son si implémenté) en fonction de projType

    sprite.setPosition(currentPosition);
    if (projectileTexture.getSize().x > 0) { // Vérifie si la texture a été chargée
        sprite.setOrigin(projectileTexture.getSize().x / 2.f, projectileTexture.getSize().y / 2.f);
    }

    // Calculer la vélocité
    sf::Vector2f direction = targetPosition - startPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        velocity = (direction / length) * moveSpeed;
    } else {
        velocity = sf::Vector2f(0, 0); // Pas de mouvement si la cible est au même endroit
        activeState = false; // Ou marquer comme inactif immédiatement
    }
    // Orienter le sprite (optionnel, surtout pour les flèches)
    if (length != 0) {
         float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
         sprite.setRotation(angle);
    }
}

void Projectile::loadResources() {
    std::string texturePath = "";
    // std::string soundPath = ""; // Pour les sons

    switch (projType) {
        case ProjectileType::ARROW:
            texturePath = "assets/textures/projectiles/arrow.png"; // REMPLACE AVEC TES CHEMINS
            // soundPath = "assets/sounds/arrow_hit.wav";
            break;
        case ProjectileType::FIREBALL:
            texturePath = "assets/textures/projectiles/fireball.png"; // REMPLACE
            // soundPath = "assets/sounds/fireball_hit.wav";
            break;
        case ProjectileType::SWORD_HIT: // Peut-être pas de texture de projectile, ou une texture d'effet
            texturePath = "assets/textures/effects/sword_slash.png"; // REMPLACE (si c'est un effet visuel)
            // soundPath = "assets/sounds/sword_hit.wav";
            this->moveSpeed = 0; // Un coup d'épée n'a pas de vitesse de projectile
            this->activeState = true; // Actif pour une courte durée (géré dans update)
            break;
        default:
            std::cerr << "Projectile: Type de projectile inconnu pour le chargement des ressources." << std::endl;
            activeState = false; // Ne peut pas fonctionner sans ressources
            return;
    }

    if (!texturePath.empty()) {
        if (!projectileTexture.loadFromFile(texturePath)) {
            std::cerr << "Erreur Projectile: Impossible de charger la texture " << texturePath << std::endl;
            activeState = false; // Ne peut pas fonctionner sans texture
        } else {
            sprite.setTexture(projectileTexture);
        }
    }

    // if (!soundPath.empty()) {
    //     if (!hitSoundBuffer.loadFromFile(soundPath)) {
    //         std::cerr << "Erreur Projectile: Impossible de charger le son " << soundPath << std::endl;
    //     } else {
    //         hitSound.setBuffer(hitSoundBuffer);
    //     }
    // }
}


void Projectile::update(float dt) {
    if (!activeState) {
        return;
    }

    // Pour SWORD_HIT, il pourrait disparaître après un court instant
    if (projType == ProjectileType::SWORD_HIT) {
        // Logique pour faire disparaître l'effet de coup d'épée après un délai,
        // ou il est désactivé immédiatement après la vérification de collision dans Game.
        // Pour l'instant, on suppose qu'il est géré par Game::checkCollisions
    } else {
        currentPosition += velocity * dt;
        sprite.setPosition(currentPosition);
    }

    // Désactiver si hors de l'écran (exemple simple)
    // Tu auras besoin des dimensions de la fenêtre/carte ici, ou d'une autre logique
    // if (currentPosition.x < -50 || currentPosition.x > SCREEN_WIDTH + 50 ||
    //     currentPosition.y < -50 || currentPosition.y > SCREEN_HEIGHT + 50) {
    //     activeState = false;
    // }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (activeState && sprite.getTexture()) { // Ne dessine que si actif et a une texture
        window.draw(sprite);
    }
}

sf::FloatRect Projectile::getBounds() const {
    return sprite.getGlobalBounds();
}

int Projectile::getDamage() const {
    return damageAmount;
}

void Projectile::setActive(bool active) {
    activeState = active;
}

bool Projectile::isActive() const {
    return activeState;
}

ProjectileType Projectile::getType() const {
    return projType;
}
