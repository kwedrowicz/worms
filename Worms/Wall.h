#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "Material.h"
#include "Cube.h"

class Wall
{
public:
	void Draw(mat4 &V);
	void BlowSphree(float x, float y, float radius);
	void BlowCylinder(float x, float y, float radius);
	Wall(int sizex, int sizey, int sizez);
	~Wall();
	float ambient[4], diffuse[4], specular[4];
	GLfloat shininess;
	std::vector<std::vector<std::vector<Cube> > > cubes;
	std::vector<Material> materials;
	int cubeVertexCount;
	glm::mat4 M = glm::mat4(1.0);
	glm::mat4 V = glm::mat4(1.0);
	int xnum;
	int ynum;
	int znum;
};
