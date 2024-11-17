//
// Created by Dragan Dragos on 15.11.2024.
//
#ifndef TANK_H
#define TANK_H

#include <utility>
#include <vector>
#include "Projectile.h"

namespace m1 {
    class TankWars; // Forward declaration to avoid unnecessary dependencies
}

namespace tank {
    class Tank {
    public:
        // Constructor
        Tank(float coordX, float coordY, float turretAngle, std::vector<float> &heightField, float heightTank, float lengthTank);

        // Public method to place the tank
        glm::mat3 placeTank();
        glm::mat3 placeTurret();
        void updateCoord(float deltaTime, bool goToRight);
        void updateTurret(float deltaTime, bool goUp);
        void addProjectile(float lengthTurret);
        const std::vector<projectile::Projectile*>& getProjectiles() const;
        glm::mat3 launchProjectile(int index, float deltaTimeSeconds);
        void setEnemyTank(Tank *enemyTank);
        std::pair<float, float> getCoordoante();
        float getTankAngle();
        void decreaseHealth();
        int getHealth();
        glm::vec3 positionStartProjectile(float lengthTurret);
        glm::vec3 getInitialProjectileVelocity() const ;
        glm::mat3 placeHealthBar();
        glm::mat3 placeBorderBar();
        void updateCoordEveryFrame();

    private:
        // private functions
        float calculateCoordY(float coordX);
        std::pair<float, float> returnTrapezoid(float x);
        void calculateTankAngle();
        bool projectileHitTank(float coordProjectileX, float coordProjectileY, bool forEnemy);
        void applyProjectileImpact(float impactX, float impactY, int RADIUS);
        // Dimensions and position

        std::pair<float, float> coordTank;
        std::vector<float> &heightField;

        // Angles for tank orientation and turret
        float tankAngle;
        float turretAngle;
        float heightTank, lengthTank;
        int health;
        // Projectiles associated with the tank
        std::vector<projectile::Projectile*> projectilesTank;
        Tank *enemyTank;
        float maxHealth;

    };
}

#endif // TANK_H
