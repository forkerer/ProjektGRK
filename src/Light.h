#pragma once

#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "Camera.h"

class Light{
public:
	glm::vec3 pos = glm::vec3 (0, 0, 0);
	glm::vec3 color = glm::vec3 (1, 0, 1);
	glm::vec3 atten = glm::vec3 (1, 0.06, 0.0018);

	Light ();
	Light (glm::vec3, glm::vec3, glm::vec3);

	void SetShaderLight (GLuint shader, int index);


};