#include "tower.h"
#include "projectile.h" // Pour créer des projectiles
#include "enemy.h"      // Pour std::shared_ptr<Enemy>
#include "player.h"     // Pour interagir avec l'or du joueur
#include <iostream>
#include <cmath> // Pour std::sqrt

// Helper pour la distance (peut être mis dans un fichier utilitaire)
static float getDistance(sf::Vector2f p1, sf::Vector2f p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

Tower::Tower(TowerType type, sf::Vector2f position)
    : currentType(type),
      towerPosition(position),
      timeUntilNextShot(0.f),
      currentLevel(1),
      maxLevel(3), 
      currentTarget(nullptr) { // Initialisation à nullptr est correcte pour un pointeur brut
    
    loadResourcesBasedOnType(); // Charge textures, sons, et stats initiales

    sprite.setPosition(towerPosition);
    if (towerTexture.getSize().x > 0) {
        sprite.setOrigin(towerTexture.getSize().x / 2.f, towerTexture.getSize().y / 2.f);
    }
    // Le coût initial est défini dans loadResourcesBasedOnType
}

void Tower::loadResourcesBasedOnType() {
    std::string texturePath = "";
    std::string shootSoundPath = "";
    std::string upgradeSoundPath = "assets/sounds/upgrade.wav"; // Son d'amélioration commun

    // Définir les stats par défaut, puis les surcharger par type
    attackRange = 0.f;
    shotsPerSecond = 0.f;
    attackDamage = 0;
    initialCost = 0;
    costToUpgrade = 0;

    switch (currentType) {
        case TowerType::ARCHER_TOWER:
            texturePath = "assets/textures/towers/archer_tower_lvl1.png"; // REMPLACE
            shootSoundPath = "assets/sounds/arrow_shoot.wav"; // REMPLACE
            attackRange = 150.f;
            shotsPerSecond = 1.0f;
            attackDamage = 10;
            initialCost = 75;
            costToUpgrade = 50;
            break;
        case TowerType::CANNON_TOWER:
            texturePath = "assets/textures/towers/cannon_tower_lvl1.png"; // REMPLACE
            shootSoundPath = "assets/sounds/cannon_shoot.wav"; // REMPLACE
            attackRange = 200.f;
            shotsPerSecond = 0.5f; // Tire moins souvent
            attackDamage = 30;     // Plus de dégâts
            initialCost = 120;
            costToUpgrade = 80;
            break;
        case TowerType::MAGE_TOWER: // Pour le type FIRE
            texturePath = "assets/textures/towers/mage_tower_lvl1.png"; // REMPLACE
            shootSoundPath = "assets/sounds/fireball_shoot.wav"; // REMPLACE
            attackRange = 180.f;
            shotsPerSecond = 0.8f;
            attackDamage = 20; // Dégâts de feu, peut-être avec effet DoT plus tard
            initialCost = 100;
            costToUpgrade = 70;
            break;
        case TowerType::MELEE_POINT:
            texturePath = "assets/textures/towers/melee_point_lvl1.png"; // REMPLACE
            // Pas de son de tir pour la mêlée, les dégâts sont appliqués différemment
            attackRange = 32.f; // Très courte portée (taille de la tuile)
            shotsPerSecond = 1.5f; // "Taux d'attaque"
            attackDamage = 15;
            initialCost = 50;
            costToUpgrade = 30;
            break;
        default:
            std::cerr << "Tower: Type de tour inconnu pour le chargement des ressources." << std::endl;
            // Charger une texture/son par défaut ou laisser vide
            texturePath = "assets/textures/towers/unknown_tower.png";
            initialCost = 9999; // Rendre non plaçable
            costToUpgrade = 9999;
            break;
    }

    if (!texturePath.empty() && !towerTexture.loadFromFile(texturePath)) {
        std::cerr << "Erreur Tower: Impossible de charger la texture " << texturePath << std::endl;
    } else {
        sprite.setTexture(towerTexture);
    }

    if (!shootSoundPath.empty() && !shootSoundBuffer.loadFromFile(shootSoundPath)) {
        std::cerr << "Erreur Tower: Impossible de charger le son de tir " << shootSoundPath << std::endl;
    } else if (!shootSoundPath.empty()){
        towerShootSound.setBuffer(shootSoundBuffer);
    }

    if (!upgradeSoundPath.empty() && !upgradeSoundBuffer.loadFromFile(upgradeSoundPath)) {
        std::cerr << "Erreur Tower: Impossible de charger le son d'amélioration " << upgradeSoundPath << std::endl;
    } else if(!upgradeSoundPath.empty()){
        towerUpgradeSound.setBuffer(upgradeSoundBuffer);
    }
}

void Tower::update(float dt,
                   const std::vector<std::shared_ptr<Enemy>>& all_enemies, // NOUVEAU
                   std::vector<std::unique_ptr<Projectile>>& game_projectiles,
                   Player& /*game_player*/) { // game_player non utilisé ici pour l'instant

    timeUntilNextShot -= dt;

    if (!currentTarget || currentTarget->isDead() || getDistance(towerPosition, currentTarget->getPosition()) > attackRange) {
        findNewTarget(all_enemies);
    }

    if (currentTarget && timeUntilNextShot <= 0.f && shotsPerSecond > 0) {
        shootAt(currentTarget, game_projectiles);
        timeUntilNextShot = 1.f / shotsPerSecond; // Réinitialise le cooldown
        if (towerShootSound.getBuffer()) { // Joue le son de tir si chargé
            towerShootSound.play();
        }
    }
}

void Tower::findNewTarget(const std::vector<std::shared_ptr<Enemy>>& all_enemies) { // NOUVEAU
    currentTarget = nullptr;
    float closestDistance = attackRange + 1.0f;

    for (const auto& enemy_ptr : all_enemies) { // enemy_ptr est maintenant std::shared_ptr<Enemy>
        if (enemy_ptr && !enemy_ptr->isDead()) {
            float dist = getDistance(towerPosition, enemy_ptr->getPosition());
            if (dist <= attackRange && dist < closestDistance) {
                closestDistance = dist;
                currentTarget = enemy_ptr; // Assignation directe shared_ptr à shared_ptr
            }
        }
    }
}

void Tower::shootAt(std::shared_ptr<Enemy> target, std::vector<std::unique_ptr<Projectile>>& game_projectiles) { // NOUVEAU
    if (!target) return;

    ProjectileType projType = ProjectileType::ARROW;
    if (currentType == TowerType::MAGE_TOWER) {
        projType = ProjectileType::FIREBALL;
    } else if (currentType == TowerType::ARCHER_TOWER) {
        projType = ProjectileType::ARROW;
    } else if (currentType == TowerType::CANNON_TOWER) {
        projType = ProjectileType::ARROW; 
    } else if (currentType == TowerType::MELEE_POINT) {
        game_projectiles.push_back(std::make_unique<Projectile>(
            ProjectileType::SWORD_HIT, towerPosition, target->getPosition(), 0.f, attackDamage
        ));
        return; 
    }
    
    float projectileSpeed = 300.f; 
    if (projType == ProjectileType::FIREBALL) projectileSpeed = 250.f;

    game_projectiles.push_back(std::make_unique<Projectile>(
        projType, towerPosition, target->getPosition(), projectileSpeed, attackDamage
    ));
}


void Tower::draw(sf::RenderWindow& window) {
    if (sprite.getTexture()) { // Ne dessine que si la texture est valide
        window.draw(sprite);
    }
    // Optionnel: dessiner la portée de la tour pour le debug ou si sélectionnée
    // sf::CircleShape rangeCircle(attackRange);
    // rangeCircle.setOrigin(attackRange, attackRange);
    // rangeCircle.setPosition(towerPosition);
    // rangeCircle.setFillColor(sf::Color(100, 100, 100, 50));
    // rangeCircle.setOutlineColor(sf::Color::White);
    // rangeCircle.setOutlineThickness(1.f);
    // window.draw(rangeCircle);
}

sf::Vector2f Tower::getPosition() const {
    return towerPosition;
}

TowerType Tower::getTowerType() const {
    return currentType;
}

int Tower::getLevel() const {
    return currentLevel;
}

int Tower::getUpgradeCost() const {
    if (currentLevel >= maxLevel) return -1; // Ne peut pas améliorer si au niveau max (ou coût infini)
    return costToUpgrade; // Ce coût devrait augmenter avec le niveau
}

bool Tower::canUpgrade(int playerGold) const {
    if (currentLevel >= maxLevel) return false;
    return playerGold >= getUpgradeCost();
}

void Tower::upgrade() {
    if (currentLevel < maxLevel) {
        currentLevel++;
        applyUpgradeStats(); // Applique les nouvelles stats
        if (towerUpgradeSound.getBuffer()) {
            towerUpgradeSound.play();
        }
        std::cout << "Tour améliorée au niveau " << currentLevel << std::endl;

        // Mettre à jour la texture pour le nouveau niveau si nécessaire
        std::string newTexturePath = "";
        switch (currentType) {
            case TowerType::ARCHER_TOWER:
                newTexturePath = "assets/textures/towers/archer_tower_lvl" + std::to_string(currentLevel) + ".png";
                break;
            case TowerType::CANNON_TOWER:
                newTexturePath = "assets/textures/towers/cannon_tower_lvl" + std::to_string(currentLevel) + ".png";
                break;
            // Ajoute d'autres cas pour d'autres types de tours
            default: break;
        }
        if (!newTexturePath.empty() && towerTexture.loadFromFile(newTexturePath)) {
            sprite.setTexture(towerTexture); // Met à jour la texture du sprite
             if (towerTexture.getSize().x > 0) { // Réajuste l'origine si la taille change
                sprite.setOrigin(towerTexture.getSize().x / 2.f, towerTexture.getSize().y / 2.f);
            }
        } else if (!newTexturePath.empty()) {
            std::cerr << "Erreur Tower Upgrade: Impossible de charger la texture " << newTexturePath << std::endl;
        }
    }
}

void Tower::applyUpgradeStats() {
    // Augmenter les stats en fonction du niveau. Ceci est un exemple simple.
    // Tu voudras une logique plus détaillée par type de tour.
    attackDamage = static_cast<int>(attackDamage * 1.25f); // +25% dégâts
    attackRange *= 1.1f; // +10% portée
    // shotsPerSecond *= 1.1f; // Optionnel: augmenter la cadence de tir
    
    // Augmenter le coût de la prochaine amélioration
    costToUpgrade = static_cast<int>(costToUpgrade * 1.5f); 
}


// Méthode statique pour obtenir le coût de placement
int Tower::getPlacementCost(TowerType type) {
    switch (type) {
        case TowerType::ARCHER_TOWER: return 75;
        case TowerType::CANNON_TOWER: return 120;
        case TowerType::MAGE_TOWER:   return 100;
        case TowerType::MELEE_POINT:  return 50;
        default: return 9999; // Coût élevé pour type inconnu
    }
}
