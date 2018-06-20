#include "CameraBase.h"

CameraBase::CameraBase () {

}

CameraBase::~CameraBase ()
{
}

void CameraBase::HandleKeys (unsigned char key, int x, int y)
{
}

void CameraBase::HandleMouse (int x, int y)
{
}

glm::mat4 CameraBase::createCameraMatrix ()
{
	return glm::mat4 ();
}

void CameraBase::SetPosition (glm::vec3 pos)
{
}

void CameraBase::SetRotation (glm::vec3 rot)
{
}

void CameraBase::SetDirection (glm::vec3 dir)
{
}
