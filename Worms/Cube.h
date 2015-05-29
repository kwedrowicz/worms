#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace glm;

class Cube
{
public:
	bool visible;
	bool broken;
	int material;
	//mat4 M;
	int vindex[8];
	Cube();
	~Cube();
};

