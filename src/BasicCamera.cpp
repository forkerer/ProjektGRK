#include "BasicCamera.h"

BasicCamera::BasicCamera () : CameraBase() {
	SetPosition (glm::vec3 (0, 0, 0));
	SetRotation (glm::vec3 (0, 0, 0));
}

BasicCamera::~BasicCamera ()
{
}

void BasicCamera::HandleKeys (unsigned char key, int x, int y) {
	switch (key)
	{
	case 'z': rotation.x -= angleSpeed; break;
	case 'x': rotation.x += angleSpeed; break;
	case 'w': position += direction * moveSpeed; break;
	case 's': position -= direction * moveSpeed; break;
	case 'd': position += glm::cross (direction, glm::vec3 (0, 1, 0)) * moveSpeed; break;
	case 'a': position -= glm::cross (direction, glm::vec3 (0, 1, 0)) * moveSpeed; break;
	case 'q': position += glm::vec3 (0, 0.2, 0); break;
	case 'e': position -= glm::vec3 (0, 0.2, 0); break;
	}
}

glm::mat4 BasicCamera::createCameraMatrix () {
	direction = glm::vec3 (cosf (rotation.x), 0.0f, sinf (rotation.x));
	glm::vec3 up = glm::vec3 (0, 1, 0);

	return Core::createViewMatrix (position, direction, up);
}

void BasicCamera::HandleMouse (int x, int y)
{
}

void BasicCamera::SetPosition (glm::vec3 pos)
{
}

void BasicCamera::SetRotation (glm::vec3 rot)
{
}

void BasicCamera::SetDirection (glm::vec3 dir)
{
}
