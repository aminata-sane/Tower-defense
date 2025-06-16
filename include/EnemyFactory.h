#pragma once
#include "Goblin.h"
#include "Orc.h"
#include "Boss.h"
#include <memory> // Pour std::unique_ptr et std::make_unique

class EnemyFactory {
public:
    enum EnemyType {
        GOBLIN,
        ORC,
        BOSS
    };

    static std::unique_ptr<Enemy> createEnemy(EnemyType type, const std::vector<sf::Vector2f>& path) {
        switch (type) {
            case GOBLIN: return std::make_unique<Goblin>(path);
            case ORC:    return std::make_unique<Orc>(path);
            case BOSS:   return std::make_unique<Boss>(path);
            default:     return nullptr; // std::unique_ptr peut aussi être nullptr
        }
    }
};
