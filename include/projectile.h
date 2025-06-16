#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Pour les sons
#include <string>
#include <memory> // Pour std::shared_ptr si on cible un ennemi

// Déclaration anticipée si Enemy.h n'est pas inclus directement
class Enemy;

// Énumération pour les types de projectiles
enum class ProjectileType {
    ARROW,
    FIREBALL,
    SWORD_HIT // Si c'est un effet de mêlée plutôt qu'un projectile distant
    // Ajoute d'autres types si nécessaire
};

class Projectile {
public:
    // Constructeur mis à jour pour prendre un type, une position de départ, une cible (position ou ennemi), et des stats
    Projectile(ProjectileType type,
                 sf::Vector2f startPosition,
                 sf::Vector2f targetPosition, // Pour les projectiles allant vers un point
                 // std::shared_ptr<Enemy> targetEnemy, // Alternative pour les projectiles à tête chercheuse
                 float speed,
                 int damage);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    int getDamage() const;
    void setActive(bool active);
    bool isActive() const;
    ProjectileType getType() const; // Pour connaître le type du projectile

private:
    sf::Sprite sprite;
    sf::Texture projectileTexture; // Chaque projectile a sa propre texture

    ProjectileType projType;
    sf::Vector2f currentPosition;
    sf::Vector2f velocity; // Calculé en fonction de la cible et de la vitesse
    float moveSpeed;
    int damageAmount;
    bool activeState; // Renommé pour éviter la confusion avec le mot-clé 'active'

    // Pour les sons (optionnel)
    // sf::SoundBuffer hitSoundBuffer;
    // sf::Sound hitSound;

    void loadResources(); // Méthode privée pour charger texture/son en fonction du type
};

#endif // PROJECTILE_H
