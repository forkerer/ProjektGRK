#include "StaticObject.h"



StaticObject::StaticObject (obj::Model *mod, GLuint tex) : Placeable(mod, tex)
{
	RecalculateMatrix ();
}


StaticObject::~StaticObject ()
{
}

void StaticObject::SetPosition (glm::vec3 position) {
	Placeable::SetPosition (position);
	RecalculateMatrix ();
}
void StaticObject::SetRotation (glm::vec3 rotation) {
	Placeable::SetRotation (rotation);
	RecalculateMatrix ();
}
void StaticObject::SetScale (glm::vec3 scal) {
	Placeable::SetScale (scal);
	RecalculateMatrix ();
}

void StaticObject::RecalculateMatrix () {
	glm::vec3 xAxis = glm::vec3 (1, 0, 0);
	glm::vec3 yAxis = glm::vec3 (0, 1, 0);
	glm::vec3 zAxis = glm::vec3 (0, 0, 1);
	modelMat = glm::translate (pos) * glm::rotate (glm::radians (rot.x), yAxis) * glm::rotate (glm::radians (rot.y), xAxis) * glm::rotate (glm::radians (rot.z), zAxis) * glm::scale (scale);
}

glm::mat4 StaticObject::GetModelMatrix () {
	return modelMat;
}