#pragma once
#include "Placeable.h"
class StaticObject :
	public Placeable
{
public:
	glm::mat4 modelMat;

	StaticObject (obj::Model *mod, GLuint tex);
	~StaticObject ();

	void RecalculateMatrix ();
	glm::mat4 GetModelMatrix () override;
	void SetPosition (glm::vec3 pos) override;
	void SetRotation (glm::vec3 rot) override;
	void SetScale (glm::vec3 scale) override;
};

