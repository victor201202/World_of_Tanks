#pragma once

#include "components/simple_scene.h"
#include "lab_camera.h"
#include "Tank.h"
#include "Building.h"
#include "Projectile.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void RenderTank(Tank tank);
        void FrameStart() override;
        void SpawnBuildings();
        void SpawnEnemy();
        void RenderBuildings();
        void RenderProjectiles();
        void ComputeProjectiles(float deltaTime);
        void ComputeCollisions();
        void RandomizeMovement(Tank* tank, float deltaTime);
        void Update(float deltaTimeSeconds) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int health);
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        //void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        int score;
        bool game_over;
        std::chrono::system_clock::time_point start_time;

        // TODO(student): If you need any other class variables, define them here.
        float length, width, radians, znear, zfar;
        bool perspective, ortho;
        std::vector<Building> buildings;
        std::vector<Projectile> projectiles;
        std::vector<Tank*> tanks;
    };
}   // namespace m1
