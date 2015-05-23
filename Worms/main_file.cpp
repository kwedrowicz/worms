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
#include "Robot.h"

using namespace std;
using namespace glm;

float *geomVertices, *geomColors, *geomNormals;
int geomVertexCount = 0;

float speed = 0; //60 stopni/s
int lastTime = 0;
float angle_float;
float scaleModifier = 0.3;

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
	
	/*glVertexPointer(3, GL_FLOAT, 0, geomVertices);
	glColorPointer(3, GL_FLOAT, 0, geomColors);
	glNormalPointer(GL_FLOAT, 0,geomNormals);
	//glDrawElements(GL_TRIANGLES, licz_fejsy, GL_UNSIGNED_INT, geomFaces);
	glDrawArrays(GL_TRIANGLES, 0, geomVertexCount);*/
	

	for (int i = 0; i < 5; i++)
	{
		M = translate(M, vec3(5.0f, 0.0f, 0.0f));
		glLoadMatrixf(value_ptr(V*M));
		glVertexPointer(3, GL_FLOAT, 0, geomVertices);
		glColorPointer(3, GL_FLOAT, 0, geomColors);
		glNormalPointer(GL_FLOAT, 0, geomNormals);
		glDrawArrays(GL_TRIANGLES, 0, geomVertexCount);
	}

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
	if (scaleModifier <= 0.0f)
		scaleModifier = 0.1f;
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


	Robot robot;
	geomVertices = &robot.vertices[0];
	geomColors = &robot.colors[0];
	geomNormals = &robot.normals[0];
	geomVertexCount = robot.vertex_count;


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
