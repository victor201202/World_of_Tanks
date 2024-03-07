#pragma once
#include "components/simple_scene.h"

class Senile
{
private:
public:
	float scale;
	glm::vec3 color;
	glm::vec3 pos;
	float rotation;
	Senile();
	void Render();
};

