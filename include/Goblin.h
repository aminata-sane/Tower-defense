#pragma once
#include "enemy.h"

class Goblin : public Enemy {
public:
    Goblin(const std::vector<sf::Vector2f>& path)
        : Enemy(path, 90.f, 50, "assets/images/goblin.png") {}

    int getMaxHp() const override { return 50; }
};
