#pragma once
#include "Model.h"
class Robot
{
public:
	Model body, left_arm, right_arm, ball, eyes;
	bool isTurnRight = false;
	bool onGround = true;
	int  direction = 1;
	float altitude = 0.0f;
	float verticalSpeed = 0.0f;
	float horizontalSpeed = 0.0f;
	float gravity = -9.8f;

	void turnFaceSide();
	void jump();
	void calculateGravity(int time);
	void Draw(glm::mat4 &view);
	void translateWhole(glm::vec3 v);

	Robot();
	~Robot();
};

