#pragma once
#include "enemy.h"

class Boss : public Enemy
{
public:
    Boss(const std::vector<sf::Vector2f> &path)
        : Enemy(path, 30.f, 300, "assets/images/boss.png") {}

    int getMaxHp() const override { return 300; }
};