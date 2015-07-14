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
	void BlowCylinder(vec4 &myPosition, float radius);
	int FetchMeshVertexIndex(bool justnormals, int cubex, int cubey, int cubez, int vx, int vy, int vz, int blockx, int blocky);
	void MeshPushSide(bool justnormals, int cubex, int cubey, int cubez, int side, int blockx, int blocky);
	void CreateMesh(int xdir, int ydir, int zdir, int blockx, int blocky);
	void DrawMesh(mat4 &V);
	void GrassyCuboid();
	void LetTheEarthPutForth();
	void AddCube(int x, int y, int z, int mat);
	float HowFarFromSurface(vec4 &myPosition);

	Wall(int sectorsizein, int sizex, int sizey, int sizez);
	~Wall();

	float ambient[4], diffuse[4], specular[4];
	GLfloat shininess;
	std::vector<std::vector<std::vector<Cube> > > cubes;
	std::vector<Material> materials;
	std::vector<std::vector<std::vector<int> > > meshIndices;
	std::vector<std::vector<std::vector<float> > > meshVertices;
	std::vector<std::vector<std::vector<float> > > meshColors;
	std::vector<std::vector<std::vector<float> > > meshNormals;
	int cubeVertexCount;
	glm::mat4 M = glm::mat4(1.0);
	glm::mat4 V = glm::mat4(1.0);
	int xnum;
	int ynum;
	int znum;
	int sectorsize;
	std::vector<std::vector<int> > currVIndex;
	std::vector<std::vector<int> > indicesNumber;
};
