#include "lab_m1/tank_wars/tank_wars.h"

#include <vector>
#include <iostream>

#include "lab_m1/tank_wars/transform2d.h"
#include "lab_m1/tank_wars/object2d.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TankWars::TankWars() : goodTank(nullptr), enemyTank(nullptr)
{
    lengthTank = 65;
    heightTank = 20;
    lengthTurret = 100;
    heightTurret = lengthTurret / 8;
    projectileLength = 30.0f;
}

TankWars::~TankWars()
{
}

float TankWars::function(float x) {
    return 400 + 20.0f * sin(0.029f * x) + 100.0f * sin(0.002f * x) + 100.0f * sin(0.0078f * x);
}

void TankWars::Init() {
    glm::ivec2 resolution = window->GetResolution(true);
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, 0.01f, 400.0f);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 50.0f));
    camera->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0.0f, 0.0f, 0.0f);
    float squareSide = 1.0f;

    // height for terrain
    for (int i = 0; i < resolution.x; i++) {
        heightField.push_back(function(i));
    }

    // initialize a tank
    float coordTankX = 500.0f;
    float coordTankY = function(coordTankX);
    float turretAngle = -M_PI / 8;

    //Tank::Tank(float coordX, float coordY, float heightTank, float length, float angle, float turretAngle)
    goodTank = new tank::Tank(coordTankX, coordTankY, turretAngle, heightField, heightTank, lengthTank);

    // initialize the enemyTank
    float coordEnemyX = 2000.0f;
    float coordEnemyY = function(coordEnemyX);
    float enemyTurretAngle = M_PI / 5;

    enemyTank = new tank::Tank(coordEnemyX, coordEnemyY, enemyTurretAngle, heightField, heightTank, lengthTank);

    goodTank->setEnemyTank(enemyTank);
    enemyTank->setEnemyTank(goodTank);

    Mesh* square = object2d::CreateSquare("square", corner, squareSide, glm::vec3(148.0f / 255.0f, 75.0f/255.0f, 44.0f / 255.0f), true);
    AddMeshToList(square);

    Mesh* border = object2d::CreateRectangle("border", lengthTurret, heightTurret, glm::vec3(-lengthTurret / 2, -heightTurret / 2, 0), false);
    AddMeshToList(border);

    Mesh* healthBar = object2d::CreateRectangle("healthBar", lengthTurret, heightTurret, glm::vec3(-lengthTurret / 2, -heightTurret / 2, 0), true);
    AddMeshToList(healthBar);

    Mesh* turret = object2d::CreateRectangle("turret", lengthTurret, heightTurret, glm::vec3(0, -heightTurret / 2, 0), true);
    AddMeshToList(turret);

    Mesh* tank = object2d::CreateTank("tank", glm::vec3(0, 0, 0), glm::vec3(-lengthTank/2, 0, 0), lengthTank, heightTank, true);
    AddMeshToList(tank);

    Mesh* enemyTank = object2d::CreateTank("enemyTank", glm::vec3(0, 0, 0), glm::vec3(-lengthTank/2, 0, 0), lengthTank, heightTank, true);
    AddMeshToList(enemyTank);

    // Mesh* projectile = object2d::CreateProjectile("projectile", projectileLength, glm::vec3(0, -projectileLength/2.5f, 0));
    // AddMeshToList(projectile);

    Mesh* projectile = object2d::CreateCircle("projectile", corner, 10, 360, glm::vec3(0, 0, 0)); // a projectile form of ball
    AddMeshToList(projectile);

    Mesh* trajectoryCircle = object2d::CreateCircle("circle", corner, 3, 360, glm::vec3(0, 0, 0));
    AddMeshToList(trajectoryCircle);

}

void TankWars::FrameStart()
{
    // RGB equivalent of the color #19AFEF
    glClearColor(133.0f / 255.0f, 242.0f / 255.0f, 255.0f / 255.0f, 1.0f);  // Normalize to 0-1 range for OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution(true);
    glViewport(0, 0, resolution.x, resolution.y);
}

void TankWars::generateTrajectoryForEnemyTank(float deltaTimeSeconds) {
    int nrPoints = 5000;  // Reduce the number of points for performance
    glm::vec3 startPositionEnemyTank = enemyTank->positionStartProjectile(lengthTurret);
    glm::vec3 startVelocityEnemyTank = enemyTank->getInitialProjectileVelocity(); // Assuming this method exists
    float timeStep = deltaTimeSeconds * 100.0f; // Match your updatePosition scaling


    for (int i = 0; i < nrPoints; i++) {
        modelMatrix = glm::mat3(1.0f);

        // Simulate position
        float t = i * timeStep; // Accumulate time
        glm::vec3 positionEnemy = startPositionEnemyTank;
        glm::vec3 velocityEnemy = startVelocityEnemyTank;

        // Add displacement to the starting position
        positionEnemy.x += velocityEnemy.x * t;
        positionEnemy.y += velocityEnemy.y * t - 0.5f * 0.1f * t * t; // Apply gravity's effect on Y-axis

        if(positionEnemy.y < heightField[(int)std::round(positionEnemy.x)]) {
            break;
        }

        modelMatrix *= transform2d::Translate(positionEnemy.x, positionEnemy.y);
        RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::generateTrajectoryForTank(float deltaTimeSeconds) {
    int nrPoints = 500;
    glm::vec3 startPosition = goodTank->positionStartProjectile(lengthTurret);
    glm::vec3 startVelocity = goodTank->getInitialProjectileVelocity(); // Assuming this method exists



    float timeStep = deltaTimeSeconds * 100.0f; // Match your updatePosition scaling

    for (int i = 0; i < nrPoints; i++) {
        glm::mat3 modelMatrix = glm::mat3(1.0f);

        // Simulate position
        float t = i * timeStep; // Accumulate time
        glm::vec3 position = startPosition;
        glm::vec3 velocity = startVelocity;

        position.x += velocity.x * t;
        position.y += velocity.y * t - 0.5f * 0.1f * t * t; // Apply gravity's effect on Y-axis

        // Stop rendering if trajectory hits the terrainws
        if (position.y < heightField[(int)std::round(position.x)]) {
            break;
        }
        modelMatrix *= transform2d::Translate(position.x, position.y);
        RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

    }
}



void TankWars::renderField() {
    for (float i = 1; i < heightField.size(); i++) {
        float x1 = i - 1;
        float x2 = i;
        float y1 = heightField[x1];
        float y2 = heightField[x2];

        modelMatrix = glm::mat3(1.0f);
        modelMatrix *= transform2d::Translate(x1, 0);

        // Calculate scale factors for width and height
        float widthScale = x2 - x1;

        // Calculate the shearing factor based on the slope between the points
        modelMatrix *= transform2d::Shearing(widthScale, y2 - y1);

        // Apply scaling to shape the terrain segment between x1, y1 and x2, y2
        modelMatrix *= transform2d::Scale(widthScale, std::max(y2 ,y1));

        // Render the square at each position to simulate the terrain
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::Update(float deltaTimeSeconds) {

    renderField();

    if(goodTank->getHealth() > 0) {
        generateTrajectoryForTank(deltaTimeSeconds);

        modelMatrix = goodTank->placeTank(); // Pass the current TankWars instance
        RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);

        // Turret for Tank
        modelMatrix = goodTank->placeTurret();
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);

        modelMatrix = goodTank->placeBorderBar();
        RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

        modelMatrix = goodTank->placeHealthBar();
        RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < goodTank->getProjectiles().size(); i++) {
            modelMatrix = goodTank->launchProjectile(i, deltaTimeSeconds);
            if (modelMatrix ==  glm::mat3(1.0f))
                continue;
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
        }
    }

    if (enemyTank->getHealth() > 0) {
        generateTrajectoryForEnemyTank(deltaTimeSeconds);
        //Enemy tank
        modelMatrix = enemyTank->placeTank();
        RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);

        //Turret for enemy tank
        modelMatrix = enemyTank->placeTurret();
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);

        modelMatrix = enemyTank->placeBorderBar();
        RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

        modelMatrix = enemyTank->placeHealthBar();
        RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < enemyTank->getProjectiles().size(); i++) {
            modelMatrix = enemyTank->launchProjectile(i, deltaTimeSeconds);
            if (modelMatrix ==  glm::mat3(1.0f))
                continue;
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
        }

    }

}

void TankWars::FrameEnd()
{

}

void TankWars::OnInputUpdate(float deltaTime, int mods)
{

    if(window->KeyHold(GLFW_KEY_D)) {
        goodTank->updateCoord(deltaTime, true);
    }
    if(window->KeyHold(GLFW_KEY_A)) {
        goodTank->updateCoord(deltaTime, false);
    }
    if(window->KeyHold(GLFW_KEY_W)) {
        goodTank->updateTurret(deltaTime, true);
    }
    if(window->KeyHold(GLFW_KEY_S)) {
        goodTank->updateTurret(deltaTime, false);
    }
    if(window->KeyHold(GLFW_KEY_RIGHT)) {
        enemyTank->updateCoord(deltaTime, true);
    }
    if(window->KeyHold(GLFW_KEY_LEFT)) {
        enemyTank->updateCoord(deltaTime, false);
    }
    if(window->KeyHold(GLFW_KEY_UP)) {
        enemyTank->updateTurret(deltaTime, true);
    }
    if(window->KeyHold(GLFW_KEY_DOWN)) {
        enemyTank->updateTurret(deltaTime, false);
    }

}

void TankWars::OnKeyPress(int key, int mods)
{

    if (window->KeyHold(GLFW_KEY_SPACE)) {
        goodTank->addProjectile(lengthTurret);
    }
    if(window->KeyHold(GLFW_KEY_ENTER)) {
        enemyTank->addProjectile(lengthTurret);
    }
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}
