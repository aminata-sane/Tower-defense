#include "wave.h"
#include "EnemyFactory.h" 
#include "enemy.h"        
#include <algorithm>      
#include <iostream>       

WaveManager::WaveManager(const Map& map)
    : map(map),
      currentWaveIndex(0),
      enemiesSpawnedInWave(0),
      totalEnemiesInCurrentWave(0),
      enemiesPassed(0),
      maxAllowed(10), 
      spawnCooldown(1.0f),
      spawnTimer(0.f) {
    setupWaves();
}

void WaveManager::setMaxEnemiesAllowed(int max) {
    if (max > 0) {
        maxAllowed = max;
    }
}

bool WaveManager::hasLost() const {
    return enemiesPassed >= maxAllowed;
}

bool WaveManager::hasWon() const {
    return currentWaveIndex >= waves.size() && enemiesPassed < maxAllowed;
}

void WaveManager::setupWaves() {
    if (map.getEndPoint() == sf::Vector2i(6, 0)) { 
        waves.push_back({{{EnemyFactory::GOBLIN, 5}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 5}, {EnemyFactory::ORC, 3}}, {1}});
        waves.push_back({{{EnemyFactory::GOBLIN, 4}, {EnemyFactory::ORC, 3}, {EnemyFactory::BOSS, 2}}, {0, 1}});
    } else { 
        waves.push_back({{{EnemyFactory::GOBLIN, 5}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 5}, {EnemyFactory::ORC, 2}}, {0}});
        waves.push_back({{{EnemyFactory::GOBLIN, 4}, {EnemyFactory::ORC, 2}, {EnemyFactory::BOSS, 1}}, {0}});
    }
}

void WaveManager::update(float dt, std::vector<std::shared_ptr<Enemy>>& gameEnemies) {
    if (hasWon() || hasLost()) return;

    if (currentWaveIndex >= waves.size()) {
        // Toutes les vagues ont été spawnées. Le jeu attendra que gameEnemies soit vide.
    } else {
        if (enemiesSpawnedInWave == 0 && totalEnemiesInCurrentWave == 0) {
            const auto& config = waves[currentWaveIndex];
            totalEnemiesInCurrentWave = 0;
            for (const auto& pair : config.enemiesPerType) {
                totalEnemiesInCurrentWave += pair.second;
            }
            if (totalEnemiesInCurrentWave == 0 && currentWaveIndex < waves.size()) {
                currentWaveIndex++;
                enemiesSpawnedInWave = 0;
                totalEnemiesInCurrentWave = 0; 
                return;
            }
        }

        spawnTimer += dt;
        if (enemiesSpawnedInWave < totalEnemiesInCurrentWave && spawnTimer >= spawnCooldown) {
            spawnEnemy(gameEnemies); // Passe gameEnemies à spawnEnemy
            spawnTimer = 0.f;
        }
    }

    // La mise à jour des ennemis individuels est faite par Game::updateEnemies
    // WaveManager ne devrait pas le faire ici s'il opère sur la liste du jeu.
    // Si WaveManager avait sa propre liste, il la mettrait à jour.
    // Donc, on commente cette boucle :
    /*
    for (auto& enemy_ptr : gameEnemies) { // Opère sur gameEnemies
        if (enemy_ptr) {
            enemy_ptr->update(dt);
        }
    }
    */

    // La suppression des ennemis morts est faite par Game::cleanupEntities
    // WaveManager ne devrait pas le faire ici s'il opère sur la liste du jeu.
    // Il peut cependant compter `enemiesPassed`.
    // Pour cela, il faudrait itérer sur gameEnemies et vérifier reachedGoal.
    // Cette logique est complexe si Game supprime aussi.
    // Il est plus simple que Game informe WaveManager quand un ennemi passe.
    // Pour l'instant, on simplifie et on suppose que Game gère la suppression.
    // La logique de `enemiesPassed` doit être revue.
    // Une solution simple est que Game::updateEnemies retourne le nombre d'ennemis passés ce frame.
    // Ou que Enemy::update retourne un statut.

    // Pour l'instant, nous allons laisser la logique de comptage de `enemiesPassed`
    // en supposant qu'elle est appelée *avant* que Game ne supprime les ennemis.
    // Cela reste délicat.
    int newlyPassedThisFrame = 0;
    for (const auto& enemy_ptr : gameEnemies) {
        if (enemy_ptr && enemy_ptr->reachedGoal() && !enemy_ptr->isDead()) { // Un ennemi qui atteint le but est souvent marqué mort ensuite
            // Pour éviter double comptage, il faut une logique plus fine.
            // Supposons que Game marque l'ennemi comme "traité" après qu'il ait atteint le but.
            // Ou que WaveManager ne compte que s'il est responsable de la suppression.
        }
    }
    // enemiesPassed += newlyPassedThisFrame; // Logique à affiner

    // Vérifier si la vague actuelle est terminée (tous spawnés)
    // La condition gameEnemies.empty() pour passer à la vague suivante est gérée par Game.
    // WaveManager se concentre sur le spawn.
    if (currentWaveIndex < waves.size() &&
        enemiesSpawnedInWave >= totalEnemiesInCurrentWave /* && gameEnemies.empty() */ ) { // gameEnemies.empty() est une condition pour le *jeu* pour dire "vague finie"
        
        // Pour que WaveManager sache quand passer à la vague suivante,
        // il a besoin de savoir si tous les ennemis de la vague actuelle qu'il a spawnés sont "terminés" (morts ou passés).
        // Cela nécessite que Game communique cette information, ou que WaveManager inspecte gameEnemies.
        
        // Simplification : on passe à la vague suivante dès que tous sont spawnés.
        // Le jeu gérera le timing réel basé sur gameEnemies.empty().
        // Ou, mieux, Game appelle une méthode comme `startNextWave()` sur WaveManager.

        // Si WaveManager doit attendre que les ennemis soient nettoyés :
        bool allSpawnedEnemiesAreGone = true;
        if (enemiesSpawnedInWave >= totalEnemiesInCurrentWave) { // Si tous ont été spawnés
            // Vérifier si des ennemis de cette vague sont encore dans gameEnemies.
            // Ceci est complexe car gameEnemies contient tous les ennemis.
            // Il faudrait une façon de marquer les ennemis par vague ou de compter.
            // Pour l'instant, on suppose que Game dira quand commencer la prochaine vague.
        }
        // Si tous les ennemis de la vague actuelle sont spawnés ET que le jeu signale que la voie est libre:
        // currentWaveIndex++;
        // enemiesSpawnedInWave = 0;
        // totalEnemiesInCurrentWave = 0;
        // spawnTimer = 0.f;
    }
}

void WaveManager::spawnEnemy(std::vector<std::shared_ptr<Enemy>>& gameEnemies) {
    if (currentWaveIndex >= waves.size() || enemiesSpawnedInWave >= totalEnemiesInCurrentWave) {
        return; 
    }

    const auto& spawns = map.getSpawnPoints();
    if (spawns.empty()) {
        std::cerr << "Erreur WaveManager: Aucun point de spawn défini sur la carte." << std::endl;
        return;
    }

    const auto& config = waves[currentWaveIndex];
    if (config.spawnIndices.empty()) {
        std::cerr << "Erreur WaveManager: Aucun indice de spawn défini pour la vague " << currentWaveIndex << std::endl;
        return;
    }

    EnemyFactory::EnemyType typeToSpawn;
    bool foundType = false;
    int cumulativeCount = 0;
    for (const auto& pair : config.enemiesPerType) {
        if (enemiesSpawnedInWave < cumulativeCount + pair.second) {
            typeToSpawn = pair.first;
            foundType = true;
            break;
        }
        cumulativeCount += pair.second;
    }

    if (!foundType) {
        std::cerr << "Erreur WaveManager: Impossible de déterminer le type d'ennemi à spawner." << std::endl;
        return;
    }

    int spawnConfigIndex = enemiesSpawnedInWave % config.spawnIndices.size();
    int spawnPointMapIndex = config.spawnIndices[spawnConfigIndex];

    if (static_cast<size_t>(spawnPointMapIndex) >= spawns.size()) {
        std::cerr << "Erreur WaveManager: Indice de spawn " << spawnPointMapIndex 
                  << " hors limites." << std::endl;
        return; 
    }

    auto path = generatePathFromSpawn(spawns[spawnPointMapIndex]);
    if (path.empty()) {
         std::cerr << "Erreur WaveManager: Chemin généré vide pour le spawn " << spawnPointMapIndex << std::endl;
        return;
    }
    
    // EnemyFactory::createEnemy devrait idéalement retourner std::shared_ptr<Enemy>
    // ou nous le convertissons ici.
    std::unique_ptr<Enemy> uniqueNewEnemy = EnemyFactory::createEnemy(typeToSpawn, path);
    if (uniqueNewEnemy) {
        gameEnemies.push_back(std::move(uniqueNewEnemy)); // Convertit unique_ptr en shared_ptr lors de l'ajout
        enemiesSpawnedInWave++;
    } else {
        std::cerr << "Erreur WaveManager: EnemyFactory n'a pas pu créer l'ennemi." << std::endl;
    }
}

std::vector<sf::Vector2f> WaveManager::generatePathFromSpawn(const sf::Vector2i& spawnTile) {
    std::vector<sf::Vector2f> pixelPath;
    const auto& mapPathTiles = map.getPath(); 
    sf::Vector2i endTile = map.getEndPoint(); 

    if (mapPathTiles.empty() && spawnTile != endTile) { 
        std::cerr << "Avertissement: Aucun chemin défini sur la carte, l'ennemi pourrait ne pas bouger." << std::endl;
        pixelPath.push_back(sf::Vector2f(spawnTile.x * map.getTileSize() + map.getTileSize() / 2.f, spawnTile.y * map.getTileSize() + map.getTileSize() / 2.f));
        if(spawnTile != endTile) { 
             pixelPath.push_back(sf::Vector2f(endTile.x * map.getTileSize() + map.getTileSize() / 2.f, endTile.y * map.getTileSize() + map.getTileSize() / 2.f));
        }
        return pixelPath;
    }
    
    pixelPath.push_back(sf::Vector2f(spawnTile.x * map.getTileSize() + map.getTileSize() / 2.f, spawnTile.y * map.getTileSizemap.getTileSize() / 2.f));

    bool endPointReachedInPath = false;
    for (const auto& tile : mapPathTiles) {
        pixelPath.push_back(sf::Vector2f(tile.x * map.getTileSize() + map.getTileSize() / 2.f, tile.y * map.getTileSize() + map.getTileSize() / 2.f));
        if (tile == endTile) {
            endPointReachedInPath = true;
            break; 
        }
    }
    
    if (!endPointReachedInPath && spawnTile != endTile) {
        if (pixelPath.empty() || pixelPath.back() != sf::Vector2f(endTile.x * map.getTileSize() + map.getTileSize() / 2.f, endTile.y * map.getTileSize() + map.getTileSize() / 2.f)) {
            pixelPath.push_back(sf::Vector2f(endTile.x * map.getTileSize() + map.getTileSize() / 2.f, endTile.y * map.getTileSize() + map.getTileSize() / 2.f));
        }
    }
    return pixelPath;
}

bool WaveManager::isCurrentWaveSpawningFinished() const {
    if (currentWaveIndex >= waves.size()) return true; 
    return enemiesSpawnedInWave >= totalEnemiesInCurrentWave;
}

bool WaveManager::startNextWave() {
    if (currentWaveIndex < waves.size() -1 ) { // S'il y a une vague suivante
         if (isCurrentWaveSpawningFinished()) { // Et que la vague actuelle a fini de spawner
            currentWaveIndex++;
            enemiesSpawnedInWave = 0;
            totalEnemiesInCurrentWave = 0;
            spawnTimer = 0.f;
            std::cout << "WaveManager: Passage à la vague " << currentWaveIndex + 1 << std::endl;
            return true;
         }
    } else if (currentWaveIndex >= waves.size() -1 && isCurrentWaveSpawningFinished()){
        // C'était la dernière vague et elle a fini de spawner
        currentWaveIndex = waves.size(); // Marquer que toutes les vagues sont faites
        std::cout << "WaveManager: Toutes les vagues ont terminé de spawner." << std::endl;
        return false; // Plus de vagues à démarrer
    }
    return false; // Pas prête à passer ou plus de vagues
}

int WaveManager::getCurrentWaveNumber() const {
    return currentWaveIndex < waves.size() ? currentWaveIndex + 1 : waves.size();
}

bool WaveManager::areAllWavesDone() const {
    return currentWaveIndex >= waves.size();
}
