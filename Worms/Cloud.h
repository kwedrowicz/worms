#pragma once
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Cloud
{
public:
	float cloudSpeed = 0.1;
	float cloudDistanceTravelled = 0;
	float radius, scaleX;
	glm::mat4 M;
	float positionY, positionX, positionZ;
	float speed;

	Cloud();

	void Draw(glm::mat4&);
};