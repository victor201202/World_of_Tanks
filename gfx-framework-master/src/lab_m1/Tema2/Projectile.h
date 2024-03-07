#pragma once
#include "components/simple_scene.h"
#include "Tank.h"
#include <chrono>
class Projectile
{
public:
	glm::vec3 pos;
	std::chrono::system_clock::time_point spawn;
	std::chrono::system_clock::time_point current;
	float scale;
	float rotation;
	float sin;
	float cos;
	Tank* parent;
	Projectile();
	Projectile(glm::vec3 pos, std::chrono::system_clock::time_point spawn, std::chrono::system_clock::time_point current, float scale, float rotation, float sin, float cos, Tank* parent);
};

