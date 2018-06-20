#pragma once

#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "Camera.h"

class CameraBase {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 direction;
	glm::vec3 side;

	CameraBase ();
	~CameraBase ();

	virtual void HandleKeys (unsigned char key, int x, int y);
	virtual void HandleMouse (int x, int y);
	virtual glm::mat4 createCameraMatrix ();
	virtual void SetPosition (glm::vec3 pos);
	virtual void SetRotation (glm::vec3 rot);
	virtual void SetDirection (glm::vec3 dir);
};