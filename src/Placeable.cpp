#include "Placeable.h"



Placeable::Placeable (obj::Model *mod, GLuint tex)
{
	model = mod;
	texture = tex;
	pos = glm::vec3 (0, 0, 0);
	rot = glm::vec3 (0, 0, 0);
	scale = glm::vec3 (1, 1, 1);
}


Placeable::~Placeable ()
{
}

void Placeable::SetPosition (glm::vec3 position) {
	pos = position;
}
void Placeable::SetRotation (glm::vec3 rotation) {
	rot = rotation;
}
void Placeable::SetScale (glm::vec3 scal) {
	scale = scal;
}

glm::mat4 Placeable::GetModelMatrix () {
	glm::vec3 xAxis = glm::vec3 (1, 0, 0);
	glm::vec3 yAxis = glm::vec3 (0, 1, 0);
	glm::vec3 zAxis = glm::vec3 (0, 0, 1);
	return glm::translate (pos) * glm::rotate (glm::radians (rot.x), yAxis) * glm::rotate (glm::radians (rot.y), xAxis) * glm::rotate (glm::radians (rot.z), zAxis) * glm::scale (scale);
}
