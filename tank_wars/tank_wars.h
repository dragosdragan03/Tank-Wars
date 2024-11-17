#pragma once

#include "Projectile.h"
#include "Tank.h"
#include "components/simple_scene.h"


namespace m1
{
    class TankWars : public gfxc::SimpleScene
    {
     public:
        TankWars();
        ~TankWars();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        // helper functions
        float function(float x);
        void renderField();
        // float CalculateCoordY(float coordX);
        glm::vec3 positionStartProjectile(float coordX, float coordY, float angleTank, float angleTurret);
        // std::pair<float, float> returnTrapezoid(float x);

     protected:
        glm::mat3 modelMatrix;
        // variables for fiels
        float terrainSize;
        std::vector<float> heightField;
        // variables for tank
        float heightTank, lengthTank;

        float lengthTurret, heightTurret;
        // variables for projectile
        float projectileLength;
        tank::Tank *goodTank, *enemyTank;
        void generateTrajectoryForEnemyTank(float deltaTimeSeconds);
        void generateTrajectoryForTank(float deltaTimeSeconds);
    };
}
