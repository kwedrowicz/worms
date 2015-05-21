#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace glm;

float *geomVertices, *geomColors;
int geomVertexCount = 0;

float speed = 60; //60 stopni/s
int lastTime = 0;
float angle_float;

//bool loadOBJ(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
bool loadOBJ(const char *path, std::vector<float> &out_vertices, std::vector<float> &out_colors, int &vertex_count)
{
	std::fstream plik;
	plik.open(path, std::ios::in);
	if (plik.good() == false)
		return false;

	std::string buf;
	float fbuf;
	while (!plik.eof())
	{
		plik >> buf;
		if (buf.compare("v") == 0)
		{
			plik >> fbuf;
			out_vertices.push_back(fbuf);
			plik >> fbuf;
			out_vertices.push_back(fbuf);
			plik >> fbuf;
			out_vertices.push_back(fbuf);
			out_colors.push_back(0.0f);
			out_colors.push_back(1.0f);
			out_colors.push_back(0.0f);
			vertex_count++;
			//		printf("%d\n", vertex_count);
		}
	}

	plik.close();
	return true;
}


void displayFrame(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 P = perspective(17.0f, 1.0f, 1.0f, 50.0f);
	mat4 V = lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 M = mat4(1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(value_ptr(V*M));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, geomVertices);
	glColorPointer(3, GL_FLOAT, 0, geomColors);
	glDrawArrays(GL_TRIANGLES, 0, geomVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
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

int main (int argc, char** argv) {

	initializeGLUT(&argc, argv);
	initializeGLEW();
	//Kod inicjuj¹cy tutaj
	glutIdleFunc(nextFrame);

	std::vector<float> dupa1, dupa2;
	bool alejaja = loadOBJ("robot.obj", dupa1, dupa2, geomVertexCount);
	if (!alejaja) return 1;
	geomVertices = &dupa1[0];
	geomColors = &dupa2[0];

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
