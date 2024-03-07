#include "Tank.h"
#include <iostream>

Tank::Tank()
{
	this->corp = Corp();
	this->senile = Senile();
	this->tun = Tun();
	this->turela = Turela();
	this->isLocal = false;
	this->pos = glm::vec3(0, 0, 0);
	this->radius = 0.8;
	this->health = 3;
}

void Tank::move(float deltaTime)
{
	senile.pos.x+= glm::sin(glm::degrees(senile.rotation)) * deltaTime;
	senile.pos.z+= glm::cos(glm::degrees(senile.rotation)) * deltaTime;

	corp.pos.x += glm::sin(glm::degrees(corp.rotation)) * deltaTime;                                  
	corp.pos.z += glm::cos(glm::degrees(corp.rotation)) * deltaTime;

	turela.pos.x += glm::sin(glm::degrees(corp.rotation)) * deltaTime;
	turela.pos.z += glm::cos(glm::degrees(corp.rotation)) * deltaTime;

	tun.pos.x += glm::sin(glm::degrees(corp.rotation)) * deltaTime;
	tun.pos.z += glm::cos(glm::degrees(corp.rotation)) * deltaTime;
}

void Tank::rotate(float deltaTime)
{
	senile.rotation += deltaTime * 0.01f;
	corp.rotation += deltaTime * 0.01f;
	turela.rotation += deltaTime * 0.01f;
	tun.rotation += deltaTime * 0.01f;
}
