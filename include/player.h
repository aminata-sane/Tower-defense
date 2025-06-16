#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(int health = 100, int gold = 50);  // Constructeur avec valeurs par défaut

    // Getters
    int getHealth() const;
    int getGold() const;
    int getScore() const;

    // Setters / opérations
    void takeDamage(int amount);
    void addGold(int amount);
    bool spendGold(int amount);
    void addScore(int points);

    // Vérifie si le joueur est encore en vie
    bool isAlive() const;

private:
    int health;
    int gold;
    int score;
};

#endif
