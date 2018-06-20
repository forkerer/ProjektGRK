#include "FPSCamera.h"
#define M_PI 3.1415926535897932384626433832795

FPSCamera::FPSCamera () : CameraBase () {
	SetPosition (glm::vec3 (0, 0, 0));
	SetRotation (glm::vec3 (0, 0, 0));
}

FPSCamera::~FPSCamera ()
{
}

void FPSCamera::HandleKeys (unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w': position += direction * moveSpeed; break;
	case 's': position -= direction * moveSpeed; break;
	case 'd': position += glm::cross (direction, glm::vec3 (0, 1, 0)) * moveSpeed; break;
	case 'a': position -= glm::cross (direction, glm::vec3 (0, 1, 0)) * moveSpeed; break;
	case 'q': position += glm::vec3 (0, 0.2, 0); break;
	case 'e': position -= glm::vec3 (0, 0.2, 0); break;
	}
}

glm::mat4 FPSCamera::createCameraMatrix () {
	lastMouseMov.x = 0;
	lastMouseMov.y = 0;
	//rotation.y = 0;
	direction.x = glm::cos (rotation.x) * glm::cos (rotation.y);
	direction.z = glm::sin (rotation.x) * glm::cos (rotation.y);
	direction.y = glm::sin (rotation.y);
	direction = glm::normalize (direction);
	//direction = glm::vec3 (cosf (rotation.x), 0.0f, sinf (rotation.x));
	up = glm::cross(direction, glm::vec3 (0, 1, 0));
	up = glm::cross (up, direction);
	up = glm::normalize (up);
	//glm::vec3 up = glm::vec3 (0, 1, 0);

	return Core::createViewMatrix (position, direction, up);
}

void FPSCamera::HandleMouse (int x, int y)
{
	lastMouseMov.x = x - lastMousePos.x;
	lastMouseMov.y = y - lastMousePos.y;

	lastMousePos.x = x;
	lastMousePos.y = y;

	rotation.x += lastMouseMov.x * pXAngle;
	rotation.y -= lastMouseMov.y * pYAngle;
	LimitAngle ();
}

void FPSCamera::LimitAngle () {
	if (rotation.x > M_PI) {
		rotation.x -= 2 * M_PI;
	}
	else if (rotation.x < -M_PI) {
		rotation.x += 2 * M_PI;
	}

	if (rotation.y > M_PI) {
		rotation.y -= 2 * M_PI;
	}
	else if (rotation.y < -M_PI) {
		rotation.y += 2 * M_PI;
	}

	if (rotation.y < (-M_PI / 2.05)) {
		rotation.y = -M_PI / 2.05;
	}
	else if (rotation.y > (M_PI / 2.05)) {
		rotation.y = M_PI / 2.05;
	}
}

void FPSCamera::SetPosition (glm::vec3 pos)
{
}

void FPSCamera::SetRotation (glm::vec3 rot)
{
}

void FPSCamera::SetDirection (glm::vec3 dir)
{
}
