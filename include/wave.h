#ifndef WAVE_H
#define WAVE_H

#include <SFML/System/Time.hpp>
#include <vector>
#include <memory> // Pour std::unique_ptr
#include "Map.h"   // Si WaveManager a besoin de la carte

// Déclaration anticipée pour éviter les dépendances cycliques si Enemy.h inclut Wave.h
class Enemy;

class WaveManager {
public:
    WaveManager(const Map& mapRef); // Constructeur, prend la carte pour les chemins par exemple

    // MODIFIÉ : Prend maintenant un vecteur d'ennemis du jeu pour y ajouter les nouveaux
    void update(float dt, std::vector<std::shared_ptr<Enemy>>& gameEnemies); // NOUVEAU

    bool allWavesCompleted() const; // AJOUTÉ : Vérifie si toutes les vagues sont terminées
    
    // bool hasLost() const; // Si WaveManager détermine la défaite
    // bool hasWon() const;  // Si WaveManager détermine la victoire (de la vague/séquence)
    // void setMaxEnemiesAllowed(int max); // Si tu as cette méthode

private:
    // Structure pour définir une vague
    struct Wave {
        int enemyTypeCount; // Ou un vecteur de types/nombres spécifiques
        int numEnemies;
        float spawnInterval;
        // ... autres propriétés de la vague
    };

    const Map& map; // Référence à la carte pour les points de spawn/chemin
    std::vector<Wave> wavesDefinition;
    int currentWaveIndex;
    float timeSinceLastSpawn;
    int enemiesSpawnedThisWave;
    // int maxEnemiesOnScreen; // Si tu as une limite

    void loadWavesFromFile(const std::string& filename); // Pour charger les définitions de vagues
    void spawnEnemy(std::vector<std::unique_ptr<Enemy>>& gameEnemies);
};

#endif // WAVE_H
