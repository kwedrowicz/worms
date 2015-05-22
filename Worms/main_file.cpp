#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace glm;

float *geomVertices, *geomColors, *geomNormals;
int geomVertexCount = 0;

float speed = 0; //60 stopni/s
int lastTime = 0;
float angle_float;
float scaleModifier = 0.3;

//bool loadOBJ(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
bool loadOBJ(const char *path, std::vector<float> &out_vertices, std::vector<float> &out_colors, std::vector<float> &out_normals, int &vertex_count)
{
	std::fstream plik;
	plik.open(path, std::ios::in);
	if (plik.good() == false)
		return false;

	vector<float> normals;
	vector<float> vertices;
	vector<int> normalsIndices;
	vector<int> vertexIndices;
	std::string buf;
	float fbuf; 
	while (!plik.eof())
	{
			plik >> buf;
		if (buf.compare("v") == 0)
		{
			plik >> fbuf;
			vertices.push_back(fbuf);
			plik >> fbuf;
			vertices.push_back(fbuf);
			plik >> fbuf;
			vertices.push_back(fbuf);
		}
		else if (buf.compare("vn") == 0)
		{
			plik >> fbuf;
			normals.push_back(fbuf);
			plik >> fbuf;
			normals.push_back(fbuf);
			plik >> fbuf;
			normals.push_back(fbuf); 
		}
		else if (buf.compare("f") == 0)
		{
			int indexa, indexb;
			for (int i = 0; i < 3; i++)
			{
				plik >> buf;
				sscanf_s(buf.c_str(), "%d//%d", &indexa, &indexb);
				vertexIndices.push_back(indexa - 1);
				normalsIndices.push_back(indexb - 1);
			}				
		}
	}
	for (int i = 0; i < normalsIndices.size(); i++)
	{
		int a = normalsIndices[i] * 3;
		int b = vertexIndices[i] * 3;
		out_vertices.push_back(vertices[b]);
		out_vertices.push_back(vertices[b+1]);
		out_vertices.push_back(vertices[b+2]);
		out_normals.push_back(normals[a]);
		out_normals.push_back(normals[a+1]);
		out_normals.push_back(normals[a+2]);
		out_colors.push_back((rand()%50)/100+0.25);
		out_colors.push_back((rand() % 50) / 100+0.5);
		out_colors.push_back((rand() % 50) / 100+0.1);
		vertex_count++;
	}
	plik.close();
	return true;
}

void displayFrame(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 P = perspective(1.5f, 1.0f, 1.0f, 50.0f);
	mat4 V = lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 M = mat4(1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);

	M = rotate(M, angle_float, vec3(0.0f, 1.0f, 0.0f));
	M = scale(M, vec3(scaleModifier, scaleModifier, scaleModifier));
	glLoadMatrixf(value_ptr(V*M));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, geomVertices);
	glColorPointer(3, GL_FLOAT, 0, geomColors);
	glNormalPointer(GL_FLOAT, 0,geomNormals);
	//glDrawElements(GL_TRIANGLES, licz_fejsy, GL_UNSIGNED_INT, geomFaces);
	glDrawArrays(GL_TRIANGLES, 0, geomVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glutSwapBuffers();

}

void nextFrame(void) {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;
	lastTime = actTime;
	angle_float += speed*interval / 1000.0;
	if (angle_float>360) angle_float -= 360;
	glutPostRedisplay();
}

void initializeGLUT(int* pargc, char** argv) {
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Program OpenGL");
	glutDisplayFunc(displayFrame);
}

void initializeGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: Nie uda³o siê zainicjowaæ biblioteki GLEW. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void keyDown(int c, int x, int y)
{
	if (c == GLUT_KEY_LEFT)
	{
		speed = -6;
	}
	else if (c == GLUT_KEY_RIGHT)
	{
		speed = 6;
	}
	else if (c == GLUT_KEY_UP)
	{
		scaleModifier += 0.5f;
	}
	else if (c == GLUT_KEY_DOWN)
	{
		scaleModifier -= 0.5f;
	}
}



void keyUp(int c, int x, int y)
{
	if (c == GLUT_KEY_LEFT || c == GLUT_KEY_RIGHT)
	{
		speed = 0;
	}
}

int main (int argc, char** argv) {

	initializeGLUT(&argc, argv);
	initializeGLEW();
	//Kod inicjuj¹cy tutaj
	glutIdleFunc(nextFrame);
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	srand(time(NULL));
	std::vector<float> dupa1, dupa2, dupa3;
	bool alejaja = loadOBJ("robot69.obj", dupa1, dupa2, dupa3,geomVertexCount);
	if (!alejaja) return 1;
	geomVertices = &dupa1[0];
	geomColors = &dupa2[0];
	geomNormals = &dupa3[0];

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
