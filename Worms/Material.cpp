#include "Material.h"
#include <fstream>
#include <string>

using namespace std;



Material::Material(float r, float g, float b)
{


	float cubeNormals1[] = {
	    -0.5, 0.5, 0.5,  //trzeci?
		0.5, 0.5, 0.5,
		-0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5
	};
	cubeNormals.insert(cubeNormals.end(),cubeNormals1,cubeNormals1+24);

	float cubeVertices1[] = {
		-0.5, 0.5, 0.5,  //trzeci?
		0.5, 0.5, 0.5,
		-0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5
	};
	cubeVertices.insert(cubeVertices.end(),cubeVertices1,cubeVertices1+24);

    unsigned int cubeIndices1[] = {
		0, 1, 2,
		1, 2, 3,
		0, 4, 6,
		0, 2, 6,
		4, 5, 6,
		5, 6, 7,
		0, 4, 5,
		0, 1, 5,
		1, 5, 7,
		1, 3, 7,
		2, 3, 7,
		2, 6, 7
	};

	cubeIndices.insert(cubeIndices.end(), cubeIndices1, cubeIndices1 + 36);

	float cubeColors1[] = {
		r, g, b, r, g, b, r, g, b, r, g, b, r, g, b, r, g, b, r, g, b, r, g, b
	};
	cubeColors.insert(cubeColors.end(), cubeColors1, cubeColors1 + 24);
}


Material::~Material()
{

}

void Material::Draw()
{

	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	
	glVertexPointer(3, GL_FLOAT, 0, &cubeVertices[0]);
	glColorPointer(3, GL_FLOAT, 0, &cubeColors[0]);
	glNormalPointer(GL_FLOAT, 0, &cubeNormals[0]);
	glLoadMatrixf(value_ptr(M));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &cubeIndices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}
