#include "Tun.h"

Tun::Tun()
{
	this->scale = 0.5f;
	this->color = glm::vec3(148.f / 255.f, 0.f / 255.f, 211.f / 255.f);
	this->rotation = 0.f;
	this->pos = glm::vec3(0, 0, 0);
}

void Tun::Render()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scale));
	//RenderSimpleMesh(meshes["senile"], shaders["LabShader"], modelMatrix, glm::vec3(169.f / 255.f, 169.f / 255.f, 169.f / 255.f));
}