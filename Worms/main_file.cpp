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
#include "Cloud.h"
#include <math.h>
#include <Windows.h>
#include <mmsystem.h>
#include <algorithm>

#define M_PI 3.14159265358979323846


using namespace std;
using namespace glm;

namespace settings{
	const int y_res = 900;
	const int x_res = 1440;
	const int x_win_pos = 0;
	const int y_win_pos = 0;
}


int someiterator = 0;
bool tnij = 0;

float speed = 0; //60 stopni/s
int lastTime = 0;
float scaleModifier = 0.3f;
Robot robot;
Robot robot2;
//Wall wall(300, 120, 40);
Wall wall(250, 100, 25);


//hmury
vector<Cloud> clouds;
const int cloudCount = 10;

vector<Robot> robots;
int active = 0;

void displayFrame(void) {
	glClearColor(0, 0.55f, 0.65f, 1);
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
	for (int i = 0; i < clouds.size(); i++)
	{
		clouds[i].Draw(V);
	}
	//missile.Draw(V,robots[0].M);
	glutSwapBuffers();
}

bool calculateCollisions();

void nextFrame(void) {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;
	lastTime = actTime;
	robots[active].M = translate(robots[active].M, vec3(speed*interval*robots[active].direction / 2000.0f, 0.0f, 0.0f));
	robots[active].ball.M = rotate(robots[active].ball.M,-1*robots[active].direction * speed/60, vec3(0.0, 0.0, 1.0));
	for (int i = 0; i < robots.size(); i++)
	{
		Model body = robots[i].body;
		body.boundingBox = body.boundingBox * robots[i].body.M2;
		float distance = wall.HowFarFromSurface(vec4((body.boundingBox.bottomLeft.x + body.boundingBox.bottomRight.x) / 2.0f, body.boundingBox.bottomRight.y, 0.0f, 0.0f));
		/*if (tnij){
			wall.BlowCylinder(vec4((body.boundingBox.bottomLeft.x + body.boundingBox.bottomRight.x) / 2.0f, body.boundingBox.bottomRight.y, 0.0f, 0.0f),10);
			tnij = 0;
		}*/

		someiterator++;
		if (someiterator<300){  //warunek i zmienna do wyrzucenia po rozwiazaniu problemu
			cout << distance << endl;
		}
		
		if (distance > 0.0f)
		{
			if (someiterator > 20){
				robots[i].onGround = false;
			}
		}
		else
		{
			robots[i].onGround = true;
			robots[i].verticalSpeed = 0.0f;
		}
		if (!robots[i].onGround)
		{
			robots[i].calculateGravity(interval);
		}
		if (robots[i].isShooting)
		{
			robots[i].calculateShot(interval);
			if (calculateCollisions())
			{
				robots[i].missileFlyTime = 0;
				robots[i].missile.M = mat4(1.0);
				robots[i].missileX = 0; 
				robots[i].missileY = 0;
				robots[i].arm_angle = robots[i].rememberAngle;
				robots[i].isShooting = false;
			}
		}
	} 
	
	for (int i = 0; i < clouds.size(); i++)
	{
		clouds[i].positionX += clouds[i].speed * interval/1000.0;
	} 
	//clouds[0].positionX += 0.1;
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
	else if (c == 'q' && !robots[active].isShooting)
	{
		tnij = 1;
		robots[active].Shot();
		PlaySound(TEXT("shooting_sound.wav"),NULL, SND_ASYNC);
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
	glEnable(GL_TEXTURE_2D);
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

struct boxes_axis
{
	float x, y;
};

struct point
{
	float x, y;
};

float pr(vec4  & p, boxes_axis & a)
{
	float projection = (p.x * a.x + p.y * a.y) / (a.x*a.x + a.y*a.y);
	float x, y;
	x = projection * a.x;
	y = projection * a.y;
	return (x * a.x + y * a.y);
}

void update_min_max(float value, float & minimum, float & maximum)
{
	if (minimum == NULL || minimum > value)
		minimum = value;
	if (maximum == NULL || maximum < value)
		maximum = value;
}

bool boxesCrossing(Model & a1, Model & b1)
{
	
	Model a = a1;
	Model b = b1;

	a.boundingBox = a.boundingBox * a.M2;
	b.boundingBox = b.boundingBox * b.M2;

	cout << a.boundingBox.topLeft.x << " " << a.boundingBox.topLeft.y << endl;
	cout << a.boundingBox.topRight.x << " " << a.boundingBox.topRight.y << endl;
	cout << a.boundingBox.bottomRight.x << " " << a.boundingBox.bottomRight.y << endl;
	cout << a.boundingBox.bottomLeft.x << " " << a.boundingBox.bottomLeft.y << endl;

	cout << endl << endl;

	cout << b.boundingBox.topLeft.x << " " << b.boundingBox.topLeft.y << endl;
	cout << b.boundingBox.topRight.x << " " << b.boundingBox.topRight.y << endl;
	cout << b.boundingBox.bottomRight.x << " " << b.boundingBox.bottomRight.y << endl;
	cout << b.boundingBox.bottomLeft.x << " " << b.boundingBox.bottomLeft.y << endl;

	//system("pause");

	boxes_axis Axis[4];
	Axis[0].x = a.boundingBox.topRight.x - a.boundingBox.topLeft.x;
	Axis[0].y = a.boundingBox.topRight.y - a.boundingBox.topLeft.y;
	Axis[1].x = a.boundingBox.topRight.x - a.boundingBox.bottomRight.x;
	Axis[1].y = a.boundingBox.topRight.y - a.boundingBox.bottomRight.y;
	Axis[2].x = b.boundingBox.topLeft.x - b.boundingBox.bottomLeft.x;
	Axis[2].y = b.boundingBox.topLeft.y - b.boundingBox.bottomLeft.y;
	Axis[3].x = b.boundingBox.topLeft.x - b.boundingBox.topRight.x;
	Axis[3].y = b.boundingBox.topLeft.y - b.boundingBox.topRight.y;

	float min_a, max_a, min_b, max_b, projection;
	min_a = max_a = min_b = max_b = NULL;
	for (int i = 0; i < 4; i++)
	{
		projection = pr(a.boundingBox.bottomLeft, Axis[i]);
		update_min_max(projection, min_a, max_a);
		projection = pr(a.boundingBox.bottomRight, Axis[i]);
		update_min_max(projection, min_a, max_a);
		projection = pr(a.boundingBox.topLeft, Axis[i]);
		update_min_max(projection, min_a, max_a);
		projection = pr(a.boundingBox.topRight, Axis[i]);
		update_min_max(projection, min_a, max_a);


		projection = pr(b.boundingBox.bottomLeft, Axis[i]);
		update_min_max(projection, min_b, max_b);
		projection = pr(b.boundingBox.bottomRight, Axis[i]);
		update_min_max(projection, min_b, max_b);
		projection = pr(b.boundingBox.topLeft, Axis[i]);
		update_min_max(projection, min_b, max_b);
		projection = pr(b.boundingBox.topRight, Axis[i]);
		update_min_max(projection, min_b, max_b);
		 
		if (min_b <= max_a && max_b >= min_a)
		{
			min_a = max_a = min_b = max_b = NULL;
		}
		else
			return false;
	}

	return true;
}

bool doOverlap(Model & a1, Model & b1)
{
	Model a = a1;
	Model b = b1;

	a.boundingBox = a.boundingBox * a.M2;
	b.boundingBox = b.boundingBox * b.M2;
 
	float r1x[] = { a.boundingBox.topLeft.x, a.boundingBox.topRight.x, a.boundingBox.bottomLeft.x, a.boundingBox.bottomRight.x };
	float r1y[] = { a.boundingBox.topLeft.y, a.boundingBox.topRight.y, a.boundingBox.bottomLeft.y, a.boundingBox.bottomRight.y };
	float r2x[] = { b.boundingBox.topLeft.x, b.boundingBox.topRight.x, b.boundingBox.bottomLeft.x, b.boundingBox.bottomRight.x };
	float r2y[] = { b.boundingBox.topLeft.y, b.boundingBox.topRight.y, b.boundingBox.bottomLeft.y, b.boundingBox.bottomRight.y };

	point l1, r1, l2, r2;
	l1.x = *min_element(r1x, r1x + 4);
	l1.y = *max_element(r1y, r1y + 4);
	r1.x = *max_element(r1x, r1x + 4);
	r1.y = *min_element(r1x, r1x + 4);

	l2.x = *min_element(r2x, r2x + 4);
	l2.y = *max_element(r2y, r2y + 4);
	r2.x = *max_element(r2x, r2x + 4);
	r2.y = *min_element(r2x, r2x + 4);


	// If one rectangle is on left side of other
	if (l1.x > r2.x || l2.x > r1.x)
		return false;

	// If one rectangle is above other
	if (l1.y < r2.y || l2.y < r1.y)
		return false;

	return true;
}


// http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604
bool calculateCollisions()
{
	for (int i = 0; i < robots.size(); i++)
	{
		cout << i << endl;
		if (i == active)
			continue;
		/*if (doOverlap(robots[active].missile, robots[i].body))
			return true;
		if (doOverlap(robots[active].missile, robots[i].ball))
			return true;*/
		if (boxesCrossing(robots[active].missile, robots[i].body))
			return true;
		if (boxesCrossing(robots[active].missile, robots[i].ball))
			return true;
	}
	return false;
}

int main (int argc, char** argv) {
	srand(time(NULL));
	
	wall.LetTheEarthPutForth();
	wall.M = scale(wall.M, vec3(0.1, 0.1, 0.1));
	wall.M = translate(wall.M, vec3(0, -70, 0));
	wall.CreateMesh(0, 0, 0);
	wall.CreateMesh(0, 0, 0);

	initializeGLUT(&argc, argv);
	initializeGLEW();
	//Kod inicjuj¹cy tutaj
	glutIdleFunc(nextFrame);
	glutKeyboardFunc(keyDown2);
	glutPassiveMotionFunc(mousePassive);
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	srand(time(NULL));

	for (int i = 0; i < cloudCount; i++)
	{
		clouds.push_back(Cloud());
	}

	robots.push_back(Robot());
	robots.push_back(Robot());

	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].M = scale(robots[i].M, vec3(0.2f, 0.2f, 0.2f));
	}

	initTextures();

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	float lpos[4] = { 1, 1, -1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//PlaySound(TEXT("intro.wav"), NULL, SND_ASYNC);
	
	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj

}
