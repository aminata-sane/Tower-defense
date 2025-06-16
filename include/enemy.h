#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string> // Pour std::string dans le constructeur si on charge la texture par chemin

class Enemy {
public:
    // Constructeur prenant un chemin de texture
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int initialHealth, const std::string& texturePath);
    virtual ~Enemy() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window);

    bool isDead() const;
    void takeDamage(int amount);
    bool reachedGoal() const;
    sf::Vector2f getPosition() const; // Retourne la position actuelle du sprite
    int getCurrentHealth() const;     // Renommé pour plus de clarté

    // Méthodes ajoutées précédemment
    virtual int getDamageToPlayer() const;
    virtual sf::FloatRect getBounds() const;
    virtual int getGoldValue() const;

     virtual int getMaxHp() const;

protected:
    sf::Sprite sprite;
    sf::Texture enemyTexture; // Membre pour stocker la texture

    sf::Vector2f currentPosition; // Position actuelle de l'ennemi

    float moveSpeed;        // Renommé pour plus de clarté
    int maxHealthPoints;    // Renommé
    int currentHealthPoints;// Renommé

    const std::vector<sf::Vector2f>& pathPoints; // Référence au chemin
    size_t currentPathTargetIndex; // Renommé

    bool hasReachedGoal; // Renommé

    void moveTowards(sf::Vector2f target, float dt); // Méthode helper pour le mouvement
};

#endif // ENEMY_H
