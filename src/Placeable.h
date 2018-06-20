#pragma once

#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>

#include "Render_Utils.h"
#include "Texture.h"

class Placeable
{
public:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	obj::Model *model;
	GLuint texture;
	GLuint secondShader;

	Placeable (obj::Model *mod, GLuint tex);
	virtual ~Placeable ();
	
	virtual void SetPosition (glm::vec3 pos);
	virtual void SetRotation (glm::vec3 rot);
	virtual void SetScale (glm::vec3 scale);
	virtual glm::mat4 GetModelMatrix ();
};

