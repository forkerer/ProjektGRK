#pragma once

#include "CameraBase.h"

class BasicCamera : public CameraBase {
public:
	float angleSpeed = 0.025f;
	float moveSpeed = 0.1f;

	BasicCamera ();
	~BasicCamera ();
	void HandleKeys (unsigned char key, int x, int y) override;
	glm::mat4 createCameraMatrix () override;
	void HandleMouse (int x, int y) override;
	void SetPosition (glm::vec3 pos) override;
	void SetRotation (glm::vec3 rot) override;
	void SetDirection (glm::vec3 dir) override;
};