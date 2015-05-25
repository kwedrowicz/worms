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


float speed = 0; //60 stopni/s
int lastTime = 0;
float scaleModifier = 0.3f;
Robot robot;
Robot robot2;

vector<Robot> robots;
int active = 0;

void displayFrame(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 P = perspective(1.5f, 1.0f, 1.0f, 50.0f);
	mat4 V = lookAt(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].Draw(V);
	}
	glutSwapBuffers();
}

void nextFrame(void) {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;
	lastTime = actTime;
	robots[active].translateWhole(vec3(speed*interval*robots[active].direction / 1000.0f, 0.0f, 0.0f));
	for (int i = 0; i < robots.size(); i++)
	{
		if (!robots[i].onGround)
		{
			robots[i].calculateGravity(interval);
		}
	}
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

void keyDown2(unsigned char c, int x, int y)
{
	if (c == ' ')
	{
		mat4 last = robots[active].right_arm.M;
		robots[active].right_arm.M = rotate(robots[active].right_arm.M, radians(10.0f), vec3(0.0f, 0.0f, 1.0f));
	}
}

void keyDown(int c, int x, int y)
{
	if (c == GLUT_KEY_LEFT)
	{
		if (robots[active].isTurnRight)
			robots[active].turnFaceSide();
		speed = 6;
	}
	else if (c == GLUT_KEY_RIGHT)
	{
		if (!robots[active].isTurnRight)
			robots[active].turnFaceSide();
		speed = -6;
	}
	else if (c == GLUT_KEY_UP)
	{
		if (robots[active].onGround)
			robots[active].jump();
	}
	else if (c == GLUT_KEY_F1)
	{
		active++;
		if (active >= robots.size())
			active = active % robots.size();
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
	glutKeyboardFunc(keyDown2);
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	srand(time(NULL));

	robots.push_back(Robot());
	robots.push_back(Robot());

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
