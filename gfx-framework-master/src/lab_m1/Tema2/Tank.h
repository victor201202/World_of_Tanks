#pragma once
#include "Senile.h"
#include "Corp.h"
#include "Turela.h"
#include "Tun.h"
#include <chrono>
#include "transform3D.h"

class Tank
{
private:
public:
	Senile senile;
	Corp corp;
	Turela turela;
	Tun tun;
	bool isLocal;
	glm::vec3 pos;
	float rotation;
	float radius;
	int health;
	std::chrono::system_clock::time_point shoot_time;
	Tank();
	void move(float deltaTime);
	void rotate(float deltaTime);
};

