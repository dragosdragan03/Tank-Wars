#include "Projectile.h"
#include <iostream>
#include <cmath> // For cos and sin

using namespace projectile;

Projectile::Projectile(glm::vec3 position, float turretAngle, float magnitudine)
    : position(position),
      viteza(glm::vec3(std::cos(M_PI / 2 + turretAngle), std::sin(M_PI / 2 + turretAngle), 0.0f) * magnitudine),
      magnitudine(magnitudine) {}

// Getter for velocity
glm::vec3 Projectile::getViteza() const {
    return viteza;
}

// Getter for position
glm::vec3 Projectile::getPosition() const {
    return position;
}

float Projectile::getAngle() const {
    // Calculate the angle using atan2
    float angle = atan2(viteza.y, viteza.x);
    // return angle;
    // Convert to degrees if necessary
    return glm::degrees(angle); // or return angle (radians) depending on your requirements
}

// Update position and optionally apply gravity
void Projectile::updatePosition(float deltaTime) {
    position.x += viteza.x * deltaTime;
    position.y += viteza.y * deltaTime;
    viteza.y -= 0.1 * deltaTime;
}
