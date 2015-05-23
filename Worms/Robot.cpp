#include "Robot.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

using namespace glm;
using namespace std;

Robot::Robot()
{
	loadObj("robot_grouped.obj");
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

void Robot::jump()
{
	cout << "HUrra" << endl;
	onGround = false;
	verticalSpeed = 8.0f;
}

void Robot::calculateGravity(int time)
{
	verticalSpeed += gravity * time / 1000.0f;
	float altitude_last = altitude;
	altitude += verticalSpeed;
	if (altitude > 0)
		M = translate(M, vec3(0.0f, verticalSpeed*time / 1000.0f, 0.0f));
	else
	{
		M = translate(M, vec3(0.0f, verticalSpeed*time*altitude_last / (1000.0f * abs(altitude_last-altitude)), 0.0f));
		onGround = true;
		altitude = 0.0f;
		verticalSpeed = 0.0f;
	}
}

