#include "wave.h"

WaveManager::WaveManager(const Map& map)
    : map(map),
      currentWaveIndex(0),
      enemiesSpawnedInWave(0),
      totalEnemies(0),
      enemiesPassed(0),
      maxAllowed(10),
      spawnCooldown(1.0f),
      spawnTimer(0.f) {
    setupWaves();
}

void WaveManager::setMaxEnemiesAllowed(int max) {
    maxAllowed = max;
}

bool WaveManager::hasLost() const {
    return enemiesPassed >= maxAllowed;
}

bool WaveManager::hasWon() const {
    return currentWaveIndex >= waves.size() && enemies.empty() && enemiesPassed < maxAllowed;
}

void WaveManager::setupWaves() {
    const auto& spawns = map.getSpawnPoints();

    if (map.getEndPoint() == sf::Vector2i(6, 0)) {
        // maps2.txt (difficile)
        waves.push_back({{{EnemyFactory::GOBLIN, 5}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 5}, {EnemyFactory::ORC, 3}}, {1}});
        waves.push_back({{{EnemyFactory::GOBLIN, 4}, {EnemyFactory::ORC, 3}, {EnemyFactory::BOSS, 2}}, {0, 1}});
    } else {
        // maps1.txt (facile)
        waves.push_back({{{EnemyFactory::GOBLIN, 5}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 5}, {EnemyFactory::ORC, 2}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 4}, {EnemyFactory::ORC, 2}, {EnemyFactory::BOSS, 1}}, {0}});
    }
}

void WaveManager::update(float dt) {
    if (currentWaveIndex >= waves.size()) return;

    spawnTimer += dt;

    if (enemiesSpawnedInWave == 0) {
        const auto& config = waves[currentWaveIndex];
        totalEnemies = 0;
        for (const auto& [type, count] : config.enemiesPerType) {
            totalEnemies += count;
        }
    }

    if (enemiesSpawnedInWave < totalEnemies && spawnTimer >= spawnCooldown) {
        spawnEnemy();
        spawnTimer = 0.f;
    }

    for (auto* enemy : enemies)
        enemy->update(dt);

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [&](Enemy* e) {
            if (e->reachedGoal()) {
                ++enemiesPassed;
                delete e;
                return true;
            }
            if (e->isDead()) {
                delete e;
                return true;
            }
            return false;
        }),
        enemies.end());

    if (isWaveFinished()) {
        currentWaveIndex++;
        enemiesSpawnedInWave = 0;
    }
}

void WaveManager::spawnEnemy() {
    if (currentWaveIndex >= waves.size()) return;

    const auto& spawns = map.getSpawnPoints();
    const auto& config = waves[currentWaveIndex];

    for (const auto& [type, count] : config.enemiesPerType) {
        for (int i = 0; i < count; ++i) {
            int spawnId = config.spawnIndices[i % config.spawnIndices.size()];
            if (spawnId >= spawns.size()) continue;

            auto path = generatePathFromSpawn(spawns[spawnId]);
            enemies.push_back(EnemyFactory::createEnemy(type, path));
            enemiesSpawnedInWave++;

            if (enemiesSpawnedInWave >= totalEnemies)
                return;
        }
    }
}

std::vector<sf::Vector2f> WaveManager::generatePathFromSpawn(const sf::Vector2i& spawn) {
    std::vector<sf::Vector2f> path;
    const auto& pathTiles = map.getPath();
    sf::Vector2i end = map.getEndPoint();

    path.push_back(sf::Vector2f(spawn.x * 64 + 32, spawn.y * 64 + 32));
    for (const auto& tile : pathTiles) {
        sf::Vector2f pos(tile.x * 64 + 32, tile.y * 64 + 32);
        path.push_back(pos);
        if (tile == end) break;
    }
    return path;
}

void WaveManager::draw(sf::RenderWindow& window) {
    for (auto* enemy : enemies)
        enemy->draw(window);
}

bool WaveManager::isWaveFinished() const {
    return enemiesSpawnedInWave >= totalEnemies && enemies.empty();
}

std::vector<Enemy*>& WaveManager::getEnemies() {
    return enemies;
}
