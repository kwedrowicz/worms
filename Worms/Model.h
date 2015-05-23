#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/glm.hpp"


class Model
{
public:
	std::vector<float> vertices, colors, normals;
	int vertex_count = 0;
	glm::mat4 M = glm::mat4(1.0);
	bool loadObj(std::string path);
	void Draw();
	Model();
	~Model();
};

