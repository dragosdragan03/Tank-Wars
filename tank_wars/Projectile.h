#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "utils/glm_utils.h"

namespace projectile {
    class Projectile {
    public:
        // Constructor to initialize the velocity and position
        Projectile(glm::vec3 position, float turretAngle, float magnitudine);

        // Getters for velocity and position
        glm::vec3 getViteza() const;
        glm::vec3 getPosition() const;
        float getAngle() const ;

        // Method to update position based on deltaTime and apply gravity
        void updatePosition(float deltaTime);

    private:
        glm::vec3 viteza;   // Velocity vector
        glm::vec3 position; // Position vector
        float magnitudine;  // Magnitude of the initial velocity
    };
}

#endif //PROJECTILE_H
