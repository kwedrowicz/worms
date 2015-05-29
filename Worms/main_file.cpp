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
#include "tga.h"
#include "Wall.h"
#include <math.h>

#define M_PI 3.14159265358979323846


using namespace std;
using namespace glm;

namespace settings{
	const int y_res = 900;
	const int x_res = 1440;
	const int x_win_pos = 0;
	const int y_win_pos = 0;
}

float speed = 0; //60 stopni/s
int lastTime = 0;
float scaleModifier = 0.3f;
Robot robot;
Robot robot2;
Wall wall(200, 70, 30);


vector<Robot> robots;
int active = 0;

void displayFrame(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float lpos[4] = { 1, 1, -1, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	mat4 P = perspective(1.5f, 1.0f, 1.0f, 50.0f);
	mat4 V = lookAt(vec3(0.0f, 0.0f, -15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	
	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].Draw(V);
		wall.DrawMesh(V);
	}
	//missile.Draw(V,robots[0].M);
	glutSwapBuffers();
}

void nextFrame(void) {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;
	lastTime = actTime;
	robots[active].M = translate(robots[active].M, vec3(speed*interval*robots[active].direction / 2000.0f, 0.0f, 0.0f));
	robots[active].ball.M = rotate(robots[active].ball.M,-1*robots[active].direction * speed/60, vec3(0.0, 0.0, 1.0));
	for (int i = 0; i < robots.size(); i++)
	{
		if (!robots[i].onGround)
		{
			robots[i].calculateGravity(interval);
		}
		if (robots[i].isShooting)
		{
			robots[i].calculateShot(interval);
		}
	}
	glutPostRedisplay();
}

void initializeGLUT(int* pargc, char** argv) {
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(settings::x_win_pos, settings::y_win_pos);
	glutInitWindowSize(settings::x_res, settings::y_res);
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
		robots[active].right_arm.M = translate(robots[active].right_arm.M, vec3(0, 0.94638f + 1.89f,0));
		robots[active].right_arm.M = rotate(robots[active].right_arm.M, radians(10.0f), vec3(0.0f, 0.0f, 1.0f));
		robots[active].arm_angle += 10.0f;
		robots[active].right_arm.M = translate(robots[active].right_arm.M, vec3(0, -0.94638f - 1.89f,0));

	}
	else if (c == 'q')
	{
		robots[active].Shot();
	}
	else if (c == 'w')
	{
		system("pause");
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
	if (c == GLUT_KEY_RIGHT)
	{
		if (!robots[active].isTurnRight)
			robots[active].turnFaceSide();
		speed = -6;
	}
	if (c == GLUT_KEY_UP)
	{
		if (robots[active].onGround)
			robots[active].jump();
	}
	if (c == GLUT_KEY_F1)
	{
		active++;
		if (active >= robots.size())
			active = active % robots.size();
	}
}

void mousePassive(int x, int y)
{
	if (!robots[active].isShooting)
	{
		float angle = atan2(settings::y_res / 2 - y, settings::x_res / 2 - x);
	angle += radians(90.0f);
	if (robots[active].isTurnRight) angle = -angle;
	//angle = angle * 180 / M_PI;
	robots[active].right_arm.M = mat4(1.0);
	robots[active].right_arm.M = translate(robots[active].right_arm.M, vec3(0, 0.94638f + 1.9f, 0));
	robots[active].right_arm.M = rotate(robots[active].right_arm.M, angle, vec3(0.0f, 0.0f, 1.0f));
		robots[active].arm_angle = angle;
	robots[active].right_arm.M = translate(robots[active].right_arm.M, vec3(0, -0.94638f - 1.9f, 0));
	}
}

void keyUp(int c, int x, int y)
{
	if (c == GLUT_KEY_LEFT || c == GLUT_KEY_RIGHT)
	{
		speed = 0;
	}
}

bool initTextures()
{
	TGAImg img;
	GLuint body, eyes, tmissile;
	if (img.Load("tex_body.tga") == IMG_OK) {
		glGenTextures(1, &body); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D, body); //Przetwarzaj uchwyt tex
		if (img.GetBPP() == 24) //Obrazek 24bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 3, img.GetWidth(), img.GetHeight(), 0,
				GL_RGB, GL_UNSIGNED_BYTE, img.GetImg());
		}
		else if (img.GetBPP() == 32) //Obrazek 32bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 4, img.GetWidth(), img.GetHeight(), 0,
				GL_RGBA, GL_UNSIGNED_BYTE, img.GetImg());
		}
	}
	else return 5;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (img.Load("tex_eyes.tga") == IMG_OK) {
		glGenTextures(1, &eyes); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D, eyes); //Przetwarzaj uchwyt tex
		if (img.GetBPP() == 24) //Obrazek 24bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 3, img.GetWidth(), img.GetHeight(), 0,
				GL_RGB, GL_UNSIGNED_BYTE, img.GetImg());
		}
		else if (img.GetBPP() == 32) //Obrazek 32bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 4, img.GetWidth(), img.GetHeight(), 0,
				GL_RGBA, GL_UNSIGNED_BYTE, img.GetImg());
		}
	}
	else return 5;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (img.Load("tex_missile.tga") == IMG_OK) {
		glGenTextures(1, &tmissile); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D, tmissile); //Przetwarzaj uchwyt tex
		if (img.GetBPP() == 24) //Obrazek 24bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 3, img.GetWidth(), img.GetHeight(), 0,
				GL_RGB, GL_UNSIGNED_BYTE, img.GetImg());
		}
		else if (img.GetBPP() == 32) //Obrazek 32bit
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 4, img.GetWidth(), img.GetHeight(), 0,
				GL_RGBA, GL_UNSIGNED_BYTE, img.GetImg());
		}
	}
	else return 5;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);


	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].body.tex_handle = body;
		robots[i].ball.tex_handle = body;
		robots[i].left_arm.tex_handle = body;
		robots[i].right_arm.tex_handle = body;
		robots[i].eyes.tex_handle = eyes;
		robots[i].missile.tex_handle = tmissile;
	}
	return true;
}

int main (int argc, char** argv) {
	wall.BlowCylinder(-30, 15, 20);
	wall.M = scale(wall.M, vec3(0.2, 0.2, 0.2));
	wall.M = translate(wall.M, vec3(0, -40, 50));
	wall.CreateMesh(0,0,0);

	initializeGLUT(&argc, argv);
	initializeGLEW();
	//Kod inicjuj¹cy tutaj
	glutIdleFunc(nextFrame);
	glutKeyboardFunc(keyDown2);
	glutPassiveMotionFunc(mousePassive);
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	srand(time(NULL));

	robots.push_back(Robot());
	robots.push_back(Robot());
	initTextures();

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
