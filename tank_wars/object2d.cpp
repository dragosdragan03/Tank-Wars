#include "object2d.h"

#include <vector>

#include "transform2d.h"
#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2d::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2d::CreateCircle(const std::string &name, glm::vec3 center, float radius, int numberOfPoints, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Add the center point of the circle
    vertices.push_back(VertexFormat(center, color));
    int centerIndex = 0;

    // Add vertices around the circle's perimeter
    for (int i = 0; i <= numberOfPoints; i++) {
        float theta = 2.0f * M_PI * i / numberOfPoints; // Angle in radians
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(center.x + x, center.y + y, center.z), color));

        // Add indices to form triangles
        if (i > 0) {
            indices.push_back(centerIndex);       // Center vertex
            indices.push_back(i);                // Current perimeter vertex
            indices.push_back(i + 1);            // Next perimeter vertex
        }
    }

    // Connect the last vertex to the first to close the circle
    indices.push_back(centerIndex);
    indices.push_back(numberOfPoints);
    indices.push_back(1);

    // Create and return the mesh
    Mesh* circle = new Mesh(name);
    circle->InitFromData(vertices, indices);
    return circle;
}


Mesh* object2d::CreateTank(
    const std::string &name,
    glm::vec3 color,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    bool fill)
{
    //TODO create the trapezoid
    glm::vec3 corner = leftBottomCorner;
    float upHeight = 2.5f * height;
    glm::vec3 buttomColorTrapezoid = glm::vec3(115.0f/255.0f, 100.0f/255.0f, 78.0f/255.0f); // green
    glm::vec3 upColorTrapezoid = glm::vec3(204.0f/255.0f, 175.0f/255.0f, 136.0f/255.0f); // brown
    glm::vec3 semicercColor = glm::vec3(204.0f/255.0f, 175.0f/255.0f, 136.0f/255.0f); // black

    std::vector<VertexFormat> vertices  =
    {
        //Bottom Trapezoid
        VertexFormat(corner, buttomColorTrapezoid), // left bottom point
        VertexFormat(corner + glm::vec3(length, 0, 0), buttomColorTrapezoid), //right bottom point
        VertexFormat(corner + glm::vec3(length + length / 2, height, 0) , buttomColorTrapezoid), // right up point
        VertexFormat(corner + glm::vec3(-length/2, height, 0), buttomColorTrapezoid), // left up point

        //Up trapezoid
        VertexFormat(corner + glm::vec3(-length, height, 0), upColorTrapezoid), //left bottom point
        VertexFormat(corner + glm::vec3(2 * length, height, 0), upColorTrapezoid), //right bottom point
        VertexFormat(corner + glm::vec3(length + length * 2 / 3, upHeight, 0), upColorTrapezoid), // right up point
        VertexFormat(corner + glm::vec3(- length * 2 / 3, upHeight, 0), upColorTrapezoid), // left up point
    };

    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4};

    int numberOfPoints = 180;
    float centerSemicercX = length / 2.0f;
    float radius = length / 1.8f;
    float centerSemicercY = upHeight;
    vertices.push_back(VertexFormat(corner + glm::vec3(centerSemicercX, centerSemicercY, 0), semicercColor)); // this is the center of the circle
    int indexCenter = 8;
    indices.push_back(indexCenter);


    for (int i = 0; i <= numberOfPoints; i++) {
        float theta = i * M_PI / numberOfPoints;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        vertices.push_back(VertexFormat(corner + glm::vec3(x + centerSemicercX, y + centerSemicercY, 0), semicercColor));
        if(i == 0) {
            indices.push_back(indexCenter + 1); // index == 9
            continue;
        } else if(i == 1) {
            indices.push_back(indexCenter + 2); // index == 10
            continue;
        }
        indices.push_back(indexCenter);
        indices.push_back(indexCenter + i); // index 10
        indices.push_back(indexCenter + i + 1); // index 11
    }

    tank->InitFromData(vertices, indices);
    return tank;
}

Mesh* object2d::CreateRectangle(
    const std::string &name,
    float length,
    float width,
    glm::vec3 leftBottomCorner,
    bool fill) {

    glm::vec3 corner = leftBottomCorner;
    glm::vec3 color = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices =
    {   VertexFormat(corner, color), // left bottom point
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // right bottom point
        VertexFormat(corner + glm::vec3(length, width, 0), color), // right bottom point
        VertexFormat(corner + glm::vec3(0, width, 0), color),
    };

    Mesh* turret = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        turret->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    turret->InitFromData(vertices, indices);
    return turret;
    // Mesh* turret = new Mesh(name);
    // std::vector<unsigned int> indices = {
    //     0, 1, 2,
    //     2, 3, 0,
    // };
    //
    // turret->InitFromData(vertices, indices);
    //
    // return turret;
}

Mesh* object2d::CreateProjectile(const std::string &name, float length,glm::vec3 leftBottomCorner) {

    glm::vec3 corner = leftBottomCorner;
    glm::vec3 color = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices =
    {
        // rectangle part
        VertexFormat(corner, color), // left bottom point
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // right bottom point
        VertexFormat(corner + glm::vec3(length, length / 2, 0), color), // right up point
        VertexFormat(corner + glm::vec3(0, length / 2, 0), color), // left up point

        //Triangle
        VertexFormat(corner + glm::vec3(length + length / 2, length / 4, 0), color)
    };

    Mesh* projectile = new Mesh(name);
    std::vector<unsigned int> indices = {
        // for rectangle
        0, 1, 2,
        2, 3, 0,

        // to make the triangle at the end fo the projectile
        1, 4, 2
    };

    projectile->InitFromData(vertices, indices);

    return projectile;
}

// Mesh* object2d::CreateRectangle(
//     const std::string &name,
//     glm::vec3 leftBottomCorner,
//     float length,
//     float height,
//     glm::vec3 color,
//     bool fill)
// {
//     glm::vec3 corner = leftBottomCorner;
//
//     std::vector<VertexFormat> vertices =
//     {
//         VertexFormat(corner, color), // left bottom point
//         VertexFormat(corner + glm::vec3(length, 0, 0), color), // right bottom point
//         VertexFormat(corner + glm::vec3(length, height, 0), color), // right up point
//         VertexFormat(corner + glm::vec3(0, height, 0), color)
//     };
//
//     Mesh* rectangle = new Mesh(name);
//     std::vector<unsigned int> indices = { 0, 1, 2, 3 };
//
//     if (!fill) {
//         rectangle->SetDrawMode(GL_LINE_LOOP);
//     } else {
//         // Draw 2 triangles. Add the remaining 2 indices
//         indices.push_back(0);
//         indices.push_back(2);
//     }
//
//     rectangle->InitFromData(vertices, indices);
//     return rectangle;
// }
