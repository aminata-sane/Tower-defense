#pragma once
#include "Goblin.h"
#include "Orc.h"
#include "Boss.h"

class EnemyFactory {
public:
    enum EnemyType {
        GOBLIN,
        ORC,
        BOSS
    };

    static Enemy* createEnemy(EnemyType type, const std::vector<sf::Vector2f>& path) {
        switch (type) {
            case GOBLIN: return new Goblin(path);
            case ORC:    return new Orc(path);
            case BOSS:   return new Boss(path);
            default:     return nullptr;
        }
    }
};
