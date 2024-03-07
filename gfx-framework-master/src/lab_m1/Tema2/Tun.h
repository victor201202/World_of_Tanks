#pragma once
#include "components/simple_scene.h"
class Tun
{
private:
public:
	float scale;
	glm::vec3 color;
	glm::vec3 pos;
	float rotation;
	Tun();
	void Render();
};

