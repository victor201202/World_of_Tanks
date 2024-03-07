#include "Projectile.h"

Projectile::Projectile()
{
	this->spawn = std::chrono::system_clock::now();
	this->current = std::chrono::system_clock::now();
	this->scale = 0.f;
	this->pos = glm::vec3();
	this->rotation = 0.f;
}

Projectile::Projectile(glm::vec3 pos, std::chrono::system_clock::time_point spawn, std::chrono::system_clock::time_point current, float scale, float rotation, float sin, float cos, Tank* parent)
{
	this->spawn = spawn;
	this->current = current;
	this->scale = scale;
	this->pos = pos;
	this->rotation = rotation;
	this->sin = sin;
	this->cos = cos;
	this->parent = parent;
}

