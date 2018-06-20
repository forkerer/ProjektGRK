#pragma once
#include "Placeable.h"
class PathFollowable :
	public Placeable
{
public:
	std::vector<glm::vec3> points;
	float timeOffset;
	float moveSpeed = 0.18;
	glm::vec3 direction;

	PathFollowable (obj::Model *mod, GLuint tex);
	~PathFollowable ();

	void Update ();
	void AddPointToPath (glm::vec3 point);
	void SetPath (std::vector<glm::vec3> pts);

	glm::mat4 GetModelMatrix () override;
};

