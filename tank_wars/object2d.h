#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object2d
{
    // Create a square with a given bottom left corner, length, and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTank(const std::string &name, glm::vec3 color, glm::vec3 leftBottomCorner, float length,float height, bool fill);
    Mesh* CreateRectangle(const std::string &name, float length, float width, glm::vec3 leftBottomCorner, bool fill);
    Mesh* CreateProjectile(const std::string &name, float length,glm::vec3 leftBottomCorner);
    Mesh* CreateCircle(const std::string &name, glm::vec3 center, float radius, int numberOfPoints, glm::vec3 color);
    // Mesh* object2d::CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color, bool fill);
}
