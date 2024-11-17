//
// Created by Dragan Dragos on 15.11.2024.
//

#include "Tank.h"
#include <iostream>

#include "tank_wars.h"
#include "transform2d.h"

using namespace tank;
using namespace std;


Tank::Tank(float coordX, float coordY, float turretAngle, std::vector<float> heightField, float heightTank, float lengthTank)
        : coordTank(coordX, coordY), turretAngle(turretAngle), heightField(heightField), heightTank(heightTank), lengthTank(lengthTank) {
        calculateTankAngle();
        health = 5;
        maxHealth = health;
}

pair<float, float> Tank::returnTrapezoid(float x) {
        float y1 =  heightField[floor(x)];
        float y2 = heightField[floor(x) + 1];
        return make_pair(y1, y2);
}

void Tank::calculateTankAngle() {
        pair<float, float> trapezoidY = returnTrapezoid(coordTank.first);
        this->tankAngle = atan2((trapezoidY.second - trapezoidY.first), (floor(coordTank.first) + 1 - floor(coordTank.first)));
}

glm::mat3 Tank::placeTank() {
        // Tank
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        calculateTankAngle();
        modelMatrix *= transform2d::Translate(coordTank.first, coordTank.second);  // Translate tank to aligned position
        modelMatrix *= transform2d::Rotate(tankAngle);
        return modelMatrix;
}

glm::mat3 Tank::placeTurret() {

        glm::mat3 modelMatrix = glm::mat3(1.0f);
        float ipotenuza = 2.5f * heightTank;
        float tranlateTurretX = sin(-tankAngle) * ipotenuza + coordTank.first;
        float tranlateTurretY = cos(tankAngle) * ipotenuza + coordTank.second;
        modelMatrix *= transform2d::Translate(tranlateTurretX, tranlateTurretY);
        modelMatrix *= transform2d::Rotate(turretAngle);
        modelMatrix *= transform2d::Rotate(glm::radians(90.0f));

        return modelMatrix;
}

glm::mat3 Tank::placeBorderBar() {
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        float ipotenuza = 2.5f * heightTank;
        float radius = lengthTank / 1.8f ;
        float offset = 25.0f;
        float tranlateTurretX = sin(-tankAngle) * (ipotenuza + radius + offset) + coordTank.first;
        float tranlateTurretY = cos(tankAngle) * (ipotenuza + radius + offset) + coordTank.second;
        modelMatrix *= transform2d::Translate(tranlateTurretX, tranlateTurretY);
        modelMatrix *= transform2d::Rotate(tankAngle);

        return modelMatrix;
}

glm::mat3 Tank::placeHealthBar() {
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        float ipotenuza = 2.5f * heightTank;
        float radius = lengthTank / 1.8f ;
        float offset = 25.0f;
        float translateTurretX = sin(-tankAngle) * (ipotenuza + radius + offset) + coordTank.first;
        float translateTurretY = cos(tankAngle) * (ipotenuza + radius + offset) + coordTank.second;

        modelMatrix *= transform2d::Translate(translateTurretX, translateTurretY);

        modelMatrix *= transform2d::Rotate(tankAngle);

        modelMatrix *= transform2d::Translate(-100 / 2, 0.0f);

        modelMatrix *= transform2d::Scale(health / maxHealth, 1.0f);

        modelMatrix *= transform2d::Translate(100 / 2, 0.0f);

        return modelMatrix;
}

float Tank::calculateCoordY(float coordX) {

        pair<float, float> trapezoidY = returnTrapezoid(coordX);
        float t = (coordX - floor(coordX)) / (floor(coordX) + 1 - floor(coordX));
        float coordY = trapezoidY.first + t * (trapezoidY.second - trapezoidY.first);
        return coordY;
}

glm::vec3 Tank::positionStartProjectile(float lengthTurret) {
        //Projectile
        float ipotenuza = 2.5f * heightTank;

        // Positioning at the end of the turret
        // Calculate the tip position of the turret for the projectile's starting point
        float translateTurretX = sin(-tankAngle) * ipotenuza + coordTank.first;
        float translateTurretY = cos(tankAngle) * ipotenuza + coordTank.second;
        float translatePrjectileX = translateTurretX + sin(-turretAngle) * lengthTurret;
        float translatePrjectileY = translateTurretY + cos(turretAngle) * lengthTurret;

        // Render projectile
        return glm::vec3(translatePrjectileX, translatePrjectileY, 0.0f);
}

void Tank::addProjectile(float lengthTurret) {

        glm::vec3 position = positionStartProjectile(lengthTurret);
        auto *projectile = new projectile::Projectile(position, turretAngle, 10.0f);
        projectilesTank.push_back(projectile);
}

void Tank::setEnemyTank(Tank *enemyTank) {
        this->enemyTank = enemyTank;
}

std::pair<float, float> Tank::getCoordoante() {
        return coordTank;
}

float Tank::getTankAngle() {
        return tankAngle;
}

glm::vec3 Tank::getInitialProjectileVelocity() const {
        return glm::vec3(
            std::cos(M_PI / 2 + turretAngle) * 10.0f,
            std::sin(M_PI / 2 + turretAngle) * 10.0f,
            0.0f
        );
}


bool Tank::projectileHitTank(float coordProjectileX, float coordProjectileY) {

        float radius = lengthTank;
        float enemyTankAngle = enemyTank->getTankAngle();
        pair<float, float> enemyCoord = enemyTank->getCoordoante();
        float ip = 2.5f * heightTank;
        float centerEnemyTankX = sin(-enemyTankAngle) * ip + enemyCoord.first;
        float centerEnemyTankY = cos(enemyTankAngle) * ip + enemyCoord.second;
        float distY = coordProjectileY - centerEnemyTankY;
        float distX = coordProjectileX - centerEnemyTankX;

        float distance = std::sqrt(distY * distY + distX * distX);
        if (distance < radius) {
                return true;
        }

        return false;

}

void Tank::decreaseHealth() {
        health -= 1;
}

int Tank::getHealth() {
        return health;
}


glm::mat3 Tank::launchProjectile(int index, float deltaTimeSeconds) {

        projectile::Projectile *iterProjectile = projectilesTank[index];
        iterProjectile->updatePosition(deltaTimeSeconds * 100.0f);
        float projectilPositionX = iterProjectile->getPosition().x;
        float projectilPositionY = iterProjectile->getPosition().y;

        glm::mat3 modelMatrix = glm::mat3(1.0f);
        pair<float, float> trapezoid = returnTrapezoid(projectilPositionX);

        // i have to verify if it heats the other tank
        bool hitTank = projectileHitTank(projectilPositionX, projectilPositionY);

        if (hitTank && enemyTank->getHealth() > 0) {
                enemyTank->decreaseHealth();
                auto it = std::find(projectilesTank.begin(), projectilesTank.end(), iterProjectile);
                projectilesTank.erase(it);
                return modelMatrix;
        }

        if (projectilPositionY < trapezoid.second) { // this means is off the field so i have to erase it
                auto it = std::find(projectilesTank.begin(), projectilesTank.end(), iterProjectile);
                projectilesTank.erase(it);
                return modelMatrix;
        }

        modelMatrix = glm::mat4(1);
        float angleProjectil = iterProjectile->getAngle();
        // Apply transformations
        modelMatrix *= transform2d::Translate(projectilPositionX, projectilPositionY);  // Position at turret end
        modelMatrix *= transform2d::Rotate(angleProjectil); // Align with turret
        modelMatrix *= transform2d::Rotate(glm::radians(90.0f)); // Rotate for direction

}

const std::vector<projectile::Projectile*>& Tank::getProjectiles() const {
        return projectilesTank;
}

void Tank::updateCoord(float deltaTime, bool goToRight) {
        if (goToRight) {
                coordTank.first += deltaTime * 100.0f;
        } else {
                coordTank.first -= deltaTime * 100.0f;
        }
        coordTank.second = calculateCoordY(coordTank.first);
}

void Tank::updateTurret(float deltaTime, bool goUp) {
        if (goUp) {
           turretAngle += deltaTime * 2.0f;
        } else {
            turretAngle -= deltaTime * 2.0f;
        }

}