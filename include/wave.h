#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <vector>
#include "EnemyFactory.h"
#include "Map.h"

struct WaveConfig {
    std::vector<std::pair<EnemyFactory::EnemyType, int>> enemiesPerType;
    std::vector<int> spawnIndices;
};

class WaveManager {
public:
    WaveManager(const Map& map);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isWaveFinished() const;
    bool hasLost() const;
    bool hasWon() const;

    void setMaxEnemiesAllowed(int max);
    std::vector<Enemy*>& getEnemies();

private:
    const Map& map;
    std::vector<Enemy*> enemies;
    std::vector<WaveConfig> waves;

    size_t currentWaveIndex;
    int enemiesSpawnedInWave;
    int totalEnemies;
    int enemiesPassed;
    int maxAllowed;

    float spawnCooldown;
    float spawnTimer;

    void spawnEnemy();
    std::vector<sf::Vector2f> generatePathFromSpawn(const sf::Vector2i& spawn);
    void setupWaves();
};

#endif
