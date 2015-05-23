#include "Robot.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

Robot::Robot()
{
	loadObj("robot_newer.obj");
	M = translate(M, vec3(0.0f, -3.5f, 0.0f));
}


Robot::~Robot()
{
}

void Robot::turnFaceSide()
{
	M = rotate(M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	isTurnRight = !isTurnRight;
	direction *= -1;
}
