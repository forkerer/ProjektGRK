#include "PathFollowable.h"



PathFollowable::PathFollowable (obj::Model *mod, GLuint tex) : Placeable (mod, tex)
{
	timeOffset = (float)(rand () % 30000);
}


PathFollowable::~PathFollowable ()
{
}

void PathFollowable::AddPointToPath (glm::vec3 point) {
	points.push_back (point);
}

void PathFollowable::SetPath (std::vector<glm::vec3> pts) {
	points = pts;
}

void PathFollowable::Update () {
	float time = (glutGet (GLUT_ELAPSED_TIME) + timeOffset) * moveSpeed / 1000.0f;
	int timePoint = (int)floorf (time);
	int t1 = timePoint % (points.size()-1);
	int t2 = (timePoint + 1) % (points.size() - 1);
	int t3 = (timePoint + 2) % (points.size() - 1);
	int t4 = (timePoint + 3) % (points.size() - 1);
	float interTime = time - floorf (time);

	direction = glm::normalize (glm::catmullRom (points[t1], points[t2], points[t3], points[t4], interTime + 0.001) - glm::catmullRom (points[t1], points[t2], points[t3], points[t4], interTime - 0.001));

	Placeable::SetPosition (glm::catmullRom (points[t1], points[t2], points[t3], points[t4], interTime));
}

glm::mat4 PathFollowable::GetModelMatrix () {
	//glm::vec3 xAxis = glm::vec3 (1, 0, 0);
	glm::vec3 yAxis = glm::vec3 (0, 1, 0);
	//glm::vec3 zAxis = glm::vec3 (0, 0, 1);

	float cameraAngleX = atan2f (direction.z, direction.x);
	//float cameraAngleY = atan2f (direction.z, direction.y);
	//float cameraAngleZ = atan2f (direction.x, direction.z);
	return  glm::translate (pos) * 
			glm::rotate (-cameraAngleX + glm::radians (rot.x), yAxis) *
			//glm::rotate (-cameraAngleY + glm::radians (rot.y), xAxis) *
			//glm::rotate (-cameraAngleZ + glm::radians (rot.z), yAxis) *
			glm::scale (scale);
}

