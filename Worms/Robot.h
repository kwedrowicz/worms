#pragma once
#include "Model.h"
class Robot :
	public Model
{
public:
	bool isTurnRight = false;
	int  direction = 1;
	void turnFaceSide();
	void jump();
	int altitude = 0;
	Robot();
	~Robot();
};

