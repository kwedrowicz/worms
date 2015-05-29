#pragma once
#include "Model.h"
class Robot
{
public:
	Model body, left_arm, right_arm, ball, eyes, missile;
	bool isTurnRight = false;
	bool onGround = true;
	bool isShooting = false;
	int  direction = 1;
	float altitude = 0.0f;
	float verticalSpeed = 0.0f;
	float horizontalSpeed = 0.0f;
	float missile_speed = 0.0f;
	float gravity = -9.8f;
	float arm_angle = 0.0f;
	float missile_translate_y = 0.77206f;
	float missile_translate_x = 0.0f;
	glm::mat4 M = glm::mat4(1.0);

	int missileFlyTime = 0;
	float missileX = 0, missileY = 0.77206f;
	bool shootingRight = false;

	void turnFaceSide();
	void jump();
	void calculateGravity(int time);
	void calculateShot(int time);
	void Draw(glm::mat4 &view);
	void Shot();

	Robot();
	~Robot();
};

