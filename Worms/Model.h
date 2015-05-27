#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/glm.hpp"


class Model
{
public:
	std::vector<float> vertices, textures, normals;
	GLfloat shininess = 0;
	float ambient[4], diffuse[4], specular[4];
	GLuint tex_handle = 0;
	int vertex_count = 0;
	glm::mat4 M = glm::mat4(1.0);
	bool loadObj(std::string path);
	void SetTexture(const GLuint);
	void Draw(glm::mat4&,glm::mat4&);
	Model();
	~Model();
};

