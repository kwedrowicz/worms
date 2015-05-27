#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"


class Material
{
public:
	std::vector<float> cubeVertices, cubeColors, cubeNormals;
	std::vector<unsigned int> cubeIndices;
	void Draw();
	glm::mat4 M = glm::mat4(1.0);
	Material(float r, float g, float b);
	~Material();
};

