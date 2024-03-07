#pragma once
#include "components/simple_scene.h"
class Turela
{
private:
public:
	float scale;
	glm::vec3 color;
	glm::vec3 pos;
	float rotation;
	Turela();
	void Render();
};

