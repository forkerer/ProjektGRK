#pragma once

#include "CameraBase.h"

class FPSCamera : public CameraBase {
public:
	float moveSpeed = 0.5f;
	float pXAngle = 0.01f;
	float pYAngle = 0.01f;
	glm::vec3 up;

	struct mousePos {
		int x = 0;
		int y = 0;
	} lastMousePos;

	struct mouseMov {
		int x = 0;
		int y = 0;
	} lastMouseMov;

	FPSCamera ();
	~FPSCamera ();
	void HandleKeys (unsigned char key, int x, int y) override;
	glm::mat4 createCameraMatrix () override;
	void HandleMouse (int x, int y) override;
	void SetPosition (glm::vec3 pos) override;
	void SetRotation (glm::vec3 rot) override;
	void SetDirection (glm::vec3 dir) override;
	void LimitAngle ();
};