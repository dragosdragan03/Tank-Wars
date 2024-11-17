#pragma once

#include "utils/glm_utils.h"


namespace transform2d
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return transpose(glm::mat3(
            1.0f, 0.0f, translateX,
            0.0f, 1.0f, translateY,
            0.0f, 0.0f, 1.0f));
    }

    inline glm::mat3 Shearing(float shearyX, float shearyY) {
        float div = shearyY / shearyX;
        return transpose(
            glm::mat3(
            1, 0, 0,
            div, 1, 0,
            0, 0, 1
            ));
    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return transpose(
            glm::mat3(
            scaleX, 0.0f, 0.0f,
            0.0f, scaleY, 0.0f,
            0.0f, 0.0f, 1.0f));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        float cosTheta = cos(radians);
        float sinTheta = sin(radians);

        // Return the rotation matrix directly
        return transpose(glm::mat3(
            cosTheta, -sinTheta, 0.0f,
            sinTheta, cosTheta, 0.0f,
            0.0f, 0.0f, 1.0f
        ));
    }

}   // namespace transform2D
