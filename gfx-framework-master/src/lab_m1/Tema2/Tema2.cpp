#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <random>

using namespace std;
using namespace m1;

#define TIME_NOW std::chrono::system_clock::now()

int GetRandomNumberInRange(const int minInclusive, const int maxInclusive)
{
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());

    std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);

    return uniformDist(randomEngine);
}


enum class MovementState
{
    GoingForward,
    GoingBackward,
    InPlaceRotationLeft,
    InPlaceRotationRight
};


MovementState GetNextMovementState(const MovementState currentState)
{
    int randomChange = GetRandomNumberInRange(0, 1);
    MovementState nextState = currentState;

    switch (currentState)
    {
    case MovementState::GoingForward:
    case MovementState::GoingBackward:
        nextState = (randomChange == 1)
            ? MovementState::InPlaceRotationLeft
            : MovementState::InPlaceRotationRight;
        break;

    case MovementState::InPlaceRotationLeft:
    case MovementState::InPlaceRotationRight:
        nextState = (randomChange == 1)
            ? MovementState::GoingForward
            : MovementState::GoingBackward;
        break;

    default:
        break;
    }

    return nextState;
}


std::string GetMovementStateName(const MovementState state)
{
    static const std::map<MovementState, std::string> kStateNames
    {
        { MovementState::GoingForward, "GoingForward" },
        { MovementState::GoingBackward, "GoingBackward" },
        { MovementState::InPlaceRotationLeft, "InPlaceRotationLeft" },
        { MovementState::InPlaceRotationRight, "InPlaceRotationRight" },
    };

    std::string s = "";

    s = kStateNames.at(state);
    return s;
}


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
Tank tank_local;
Tank enemy;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    tank_local.isLocal = true;
    renderCameraTarget = false;
    enemy.shoot_time = TIME_NOW;
    tank_local.shoot_time = TIME_NOW;
    start_time = TIME_NOW;
    score = 0;
    game_over = false;

    tanks.push_back(&enemy);
    tanks.push_back(&tank_local);
    SpawnEnemy();

    SpawnBuildings();
    camera = new implemented::Camera();
    camera->distanceToTarget = glm::distance(glm::vec2(0, -4), glm::vec2(tank_local.corp.pos.x, tank_local.corp.pos.z));
    camera->Set(glm::vec3(0, 2, -4), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "senile4.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("anime");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "anime.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("floor");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    radians = RADIANS(60);
    width = 20.f;
    length = 20.f;
    znear = 0.01f;
    zfar = 200.0f;
    projectionMatrix = glm::perspective(radians, window->props.aspectRatio, znear, zfar);
    perspective = true;
    ortho = false;
}


void Tema2::SpawnBuildings()
{
    int no_buildings = 7 + glm::rand() % 8;
    float rand_x, rand_y, rand_z, rand_scale_x, rand_scale_y, rand_scale_z;
    int sign;
    for (int i = 0; i < no_buildings; i++)
    {
        rand_scale_x = 2 + glm::rand() % 4;
        rand_scale_y = 3 + glm::rand() % 4;
        rand_scale_z = 2 + glm::rand() % 4;
        sign = glm::rand() % 2;
        if (sign != 0)
            rand_x = 5 + (glm::rand() % 14) - rand_scale_x / 2;
        else
            rand_x = -5 - (glm::rand() % 14) + rand_scale_x / 2;

        rand_y = rand_scale_y / 2;

        sign = glm::rand() % 2;
        if (sign != 0)
            rand_z = 5 + (glm::rand() % 14) - rand_scale_z / 2;
        else
            rand_z = -5 - (glm::rand() % 14) + rand_scale_z / 2;

        buildings.emplace_back(glm::vec3(rand_scale_x, rand_scale_y, rand_scale_z), glm::vec3(rand_x, rand_y, rand_z));
    }
}

void Tema2::SpawnEnemy()
{
    float rand_x, rand_y, rand_z, rand_scale_x, rand_scale_y, rand_scale_z;
    int sign;
    sign = glm::rand() % 2;
    if (sign != 0)
        rand_x = 15 + (glm::rand() % 11);
    else
        rand_x = -15 - (glm::rand() % 11);

    sign = glm::rand() % 2;
    if (sign != 0)
        rand_z = 15 + (glm::rand() % 11);
    else
        rand_z = -15 - (glm::rand() % 11);

    Tank tank;
    tank.corp.pos.x = rand_x;
    tank.corp.pos.z = rand_z;
    tank.senile.pos.x = rand_x;
    tank.senile.pos.z = rand_z;
    tank.turela.pos.x = rand_x;
    tank.turela.pos.z = rand_z;
    tank.tun.pos.x = rand_x;
    tank.tun.pos.z = rand_z;
    enemy = tank;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderTank(Tank tank)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.senile.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.senile.scale));
        modelMatrix *= transform3D::RotateOY(tank.senile.rotation);
        RenderSimpleMesh(meshes["senile"], shaders["LabShader"], modelMatrix, tank.senile.color, tank.health);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.corp.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.corp.scale));
        modelMatrix *= transform3D::RotateOY(tank.corp.rotation);
        RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, tank.corp.color, tank.health);
    }

    {
        glm::vec3 traget_vec = glm::normalize(tank_local.corp.pos - tank.turela.pos);
        glm::quat rot_quat = glm::rotation(glm::vec3(0, 0, 1), traget_vec);
        auto rot_matrix = glm::toMat4(rot_quat);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.turela.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.turela.scale));
        auto distance = glm::distance(enemy.corp.pos, tank_local.corp.pos);
        if (tank.isLocal)
            modelMatrix *= transform3D::RotateOY(tank.turela.rotation);
        else if (distance <= 11 && !tank.isLocal)
            modelMatrix *= rot_matrix;
        else
            modelMatrix *= transform3D::RotateOY(tank.turela.rotation);
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix, tank.turela.color, tank.health);
    }

    {
        glm::vec3 traget_vec = glm::normalize(tank_local.corp.pos - tank.tun.pos);
        glm::quat rot_quat = glm::rotation(glm::vec3(0, 0, 1), traget_vec);
        auto rot_matrix = glm::toMat4(rot_quat);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.tun.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.tun.scale));
        auto distance = glm::distance(enemy.corp.pos, tank_local.corp.pos);
        if (tank.isLocal)
            modelMatrix *= transform3D::RotateOY(tank.tun.rotation);
        else if (distance <= 11 && !tank.isLocal)
            modelMatrix *= rot_matrix;
        else
            modelMatrix *= transform3D::RotateOY(tank.turela.rotation);

        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix, tank.tun.color, tank.health);
    }

    /*if (tank.isLocal)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, anime.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        modelMatrix *= transform3D::RotateOY(anime.rotation);
        RenderMesh(meshes["anime"], shaders["Simple"], modelMatrix);
    }*/
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int health)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
    glUniform1i(glGetUniformLocation(shader->program, "health"), health);

    mesh->Render();
}

void Tema2::ComputeProjectiles(float deltaTime)
{
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(TIME_NOW - enemy.shoot_time);
    auto distance = glm::distance(enemy.corp.pos, tank_local.corp.pos);
    if (duration.count() >= 3 && distance <= 11)
    {
        glm::vec3 tragetVec = glm::normalize(tank_local.corp.pos - enemy.turela.pos);
        glm::quat rotQuat = glm::rotation(glm::vec3(0, 0, 1), tragetVec);
        auto rotationMatrix = glm::toMat4(rotQuat);

        enemy.shoot_time = TIME_NOW;
        auto pos = enemy.tun.pos;
        pos.y += 0.6;
        pos.x += -rotationMatrix[0][2];
        pos.z += rotationMatrix[0][0];
        projectiles.emplace_back(pos, TIME_NOW, TIME_NOW, 0.25f, enemy.tun.rotation, -rotationMatrix[0][2], rotationMatrix[0][0], &enemy);
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [duration = std::chrono::seconds()](Projectile projectile) mutable {
            duration = std::chrono::duration_cast<std::chrono::seconds>(projectile.current - projectile.spawn);
            return duration.count() >= 5;
        }), projectiles.end());

    for (int i = 0; i < projectiles.size(); i++)
    {
        projectiles[i].pos.x += projectiles[i].sin * deltaTime * 10;
        projectiles[i].pos.z += projectiles[i].cos * deltaTime * 10;
        projectiles[i].current = std::chrono::system_clock::now();
    }
}

void Tema2::ComputeCollisions()
{
    for (int i = 0; i < projectiles.size(); i++)
    {
        auto projectile = projectiles[i];
        for (int j = 0; j < buildings.size(); j++)
        {
            auto building = buildings[j];
            if (projectile.pos.x >= building.pos.x - building.scale.x / 2 &&
                projectile.pos.x <= building.pos.x + building.scale.x / 2 &&
                projectile.pos.z >= building.pos.z - building.scale.z / 2 &&
                projectile.pos.z <= building.pos.z + building.scale.z / 2)
            {
                projectiles.erase(projectiles.begin() + i);
                i--;
                break;
            }
        }

        for (int j = 0; j < tanks.size(); j++)
        {
            auto tank = tanks[j];
            if (projectile.parent == tank)
                continue;

            if (projectile.pos.x >= tank->corp.pos.x - tank->radius &&
                projectile.pos.x <= tank->corp.pos.x + tank->radius &&
                projectile.pos.z >= tank->corp.pos.z - tank->radius &&
                projectile.pos.z <= tank->corp.pos.z + tank->radius)
            {
                tank->health--;
                if (tank == &enemy && tank->health == 0)
                {
                    SpawnEnemy();
                    score++;
                }
                if (tank == &tank_local && tank->health == 0)
                {
                    std::cout << "ai murit :(\n";
                    game_over = true;
                    tank->health = 1;
                }
                projectiles.erase(projectiles.begin() + i);
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < tanks.size() - 1; i++)
    {
        for (int j = i + 1; j < tanks.size(); j++)
        {
            auto tank1 = tanks[i];
            auto tank2 = tanks[j];
            float distance = glm::distance(tank1->corp.pos, tank2->corp.pos);

            if (distance < tank1->radius + tank2->radius)
            {
                float p = tank1->radius + tank2->radius - distance;
                auto dif = tank2->corp.pos - tank1->corp.pos;
                auto P = p * glm::normalize(dif);

                tank1->corp.pos += P * -0.5f;
                tank2->corp.pos += P * 0.5f;
                tank1->senile.pos += P * -0.5f;
                tank2->senile.pos += P * 0.5f;
                tank1->turela.pos += P * -0.5f;
                tank2->turela.pos += P * 0.5f;
                tank1->tun.pos += P * -0.5f;
                tank2->tun.pos += P * 0.5f;

                if (tank1 == &tank_local)
                    camera->position += P * -0.5f;
                else
                    camera->position += P * +0.5f;
            }
        }
    }

    for (int i = 0; i < tanks.size(); i++)
    {
        auto tank = tanks[i];
        for (int j = 0; j < buildings.size(); j++)
        {
            auto building = buildings[j];
            if (building.pos.z - building.scale.z / 2 - tank->radius < tank->corp.pos.z && tank->corp.pos.z < building.pos.z + building.scale.z / 2 + tank->radius
                && tank->corp.pos.x < building.pos.x + building.scale.x / 2 + tank->radius && tank->corp.pos.x > building.pos.x - building.scale.x / 2 - tank->radius)
            {
                glm::vec3 tankToBuilding = glm::normalize(building.pos - tank->corp.pos);
                glm::vec3 oppositeDirection = -tankToBuilding;
                float p = (tank->radius + building.scale.x) * 0.05;
                tank->corp.pos.x += oppositeDirection.x * p;
                tank->corp.pos.z += oppositeDirection.z * p;
                tank->senile.pos.x += oppositeDirection.x * p;
                tank->senile.pos.z += oppositeDirection.z * p;
                tank->turela.pos.x += oppositeDirection.x * p;
                tank->turela.pos.z += oppositeDirection.z * p;
                tank->tun.pos.x += oppositeDirection.x * p;
                tank->tun.pos.z += oppositeDirection.z * p;
                if (tank == &tank_local)
                {
                    camera->position.x += oppositeDirection.x * p;
                    camera->position.z += oppositeDirection.z * p;
                }
            }

        }
    }
}

void Tema2::RenderProjectiles()
{
    for (auto projectile : projectiles)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectile.pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(projectile.scale));
        RenderSimpleMesh(meshes["projectile"], shaders["LabShader"], modelMatrix, glm::vec3(153 / 255.f, 0.f / 255.f, 0.f / 255.f), 3);
    }
}

void Tema2::RenderBuildings()
{
    for (auto building : buildings)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, building.pos);
        modelMatrix = glm::scale(modelMatrix, building.scale);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(169.f / 255.f, 169.f / 255.f, 169.f / 255.f), 3);

        /* {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(building.pos.x, building.pos.y, building.pos.z - building.scale.z /2 - 0.01));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(building.scale.y));
            RenderMesh(meshes["anime"], shaders["Simple"], modelMatrix);
        }
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(building.pos.x, building.pos.y, building.pos.z + building.scale.z / 2 + 0.01));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(building.scale.y));
            RenderMesh(meshes["anime"], shaders["Simple"], modelMatrix);
        }*/
    }
}

void Tema2::RandomizeMovement(Tank* tank, float deltaTime)
{
    static MovementState state = MovementState::GoingForward;
    static auto move_time = TIME_NOW;
    static int rand_duration = 1 + glm::rand() % 3;

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(TIME_NOW - move_time);
    if (duration.count() >= rand_duration)
    {
        state = GetNextMovementState(state);
        move_time = TIME_NOW;
        if (state == MovementState::GoingForward || state == MovementState::GoingBackward)
            rand_duration = 2 + glm::rand() % 5;
        else
            rand_duration = 1 + glm::rand() % 3;
    }

    if (state == MovementState::GoingForward)
        tank->move(deltaTime);
    else if (state == MovementState::GoingBackward)
        tank->move(-deltaTime);
    else if (state == MovementState::InPlaceRotationRight)
        tank->rotate(-deltaTime);
    else if (state == MovementState::InPlaceRotationLeft)
        tank->rotate(deltaTime);
}


void Tema2::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(200));
        RenderSimpleMesh(meshes["floor"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 139.f / 255.f), 3);
    }

    if (!game_over)
    {
        RandomizeMovement(&enemy, deltaTimeSeconds);
        RenderBuildings();
        RenderTank(enemy);
        ComputeProjectiles(deltaTimeSeconds);
        ComputeCollisions();
        RenderProjectiles();
    }
    RenderTank(tank_local);
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(TIME_NOW - start_time);
    if (duration.count() >= 75 && !game_over)
    {
        std::cout << "score: " << score << '\n';
        game_over = true;
    }
}


void Tema2::FrameEnd()
{

}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            if (!game_over)
            {
                tank_local.move(5 * deltaTime);
                camera->MoveForward(5 * deltaTime);
            }
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            if (!game_over)
            {
                tank_local.move(5 * -deltaTime);
                camera->MoveForward(5 * -deltaTime);
            }
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            if (!game_over)
            {
                tank_local.rotate(-deltaTime);
                camera->RotateThirdPerson_OY(-deltaTime * 0.01f);
            }
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            if (!game_over)
            {
                tank_local.rotate(deltaTime);
                camera->RotateThirdPerson_OY(deltaTime * 0.01f);
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        if (!game_over)
        {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(TIME_NOW - tank_local.shoot_time);
            if (duration.count() >= 2)
            {
                tank_local.shoot_time = TIME_NOW;
                auto pos = tank_local.tun.pos;
                pos.y += 0.6;
                pos.x += glm::sin(glm::degrees(tank_local.tun.rotation));
                pos.z += glm::cos(glm::degrees(tank_local.tun.rotation));
                projectiles.emplace_back(pos, TIME_NOW, TIME_NOW, 0.25f, tank_local.tun.rotation, glm::sin(glm::degrees(tank_local.tun.rotation)), glm::cos(glm::degrees(tank_local.tun.rotation)), &tank_local);
            }
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    static float last_delta = 0;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.00008f;
        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
            last_delta += (float)-deltaX * sensivityOX;
        }
    }
    else if (renderCameraTarget)
    {
        renderCameraTarget = false;
        camera->RotateThirdPerson_OY(-last_delta);
        last_delta = 0;
    }
    else
    {
        float sensivityOX = 0.00007f;
        tank_local.tun.rotation -= deltaX * sensivityOX;
        tank_local.turela.rotation -= deltaX * sensivityOX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}