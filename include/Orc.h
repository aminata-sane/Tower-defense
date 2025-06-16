#pragma once
#include "enemy.h"

class Orc : public Enemy
{
public:
    Orc(const std::vector<sf::Vector2f> &path)
        : Enemy(path, 60.f, 100, "assets/images/orc.png") {}

    int getMaxHp() const override { return 100; }
};