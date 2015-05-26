#include "Robot.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

using namespace glm;
using namespace std;

Robot::Robot()
{
	body.loadObj("tex_body.obj");
	right_arm.loadObj("tex_right_arm.obj");
	left_arm.loadObj("tex_left_arm.obj");
	ball.loadObj("tex_ball.obj");
	eyes.loadObj("tex_eyes.obj");
	eyes.shininess = 1.0f;
	for (int i = 0; i < 3; i++)
	{
		eyes.ambient[i] = 1;
		eyes.specular[i] = 1;
		eyes.diffuse[i] = 1;
	}
	/*left_arm = right_arm;
	left_arm.M = rotate(left_arm.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));*/
	translateWhole(vec3(0.0f, -3.5f, 0.0f));
}


Robot::~Robot()
{
}

void Robot::turnFaceSide()
{
	body.M = rotate(body.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	right_arm.M = rotate(right_arm.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	left_arm.M = rotate(left_arm.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	ball.M = rotate(ball.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	eyes.M = rotate(eyes.M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	isTurnRight = !isTurnRight;
	direction *= -1;
}

void Robot::jump()
{
	onGround = false;
	verticalSpeed = 8.0f;
}

void Robot::calculateGravity(int time)
{
	verticalSpeed += gravity * time / 1000.0f;
	float altitude_last = altitude;
	altitude += verticalSpeed;
	if (altitude > 0)
		translateWhole(vec3(0.0f, verticalSpeed*time / 1000.0f, 0.0f));
	else
	{
		translateWhole(vec3(0.0f, verticalSpeed*time*altitude_last / (1000.0f * abs(altitude_last - altitude)), 0.0f));
		onGround = true;
		altitude = 0.0f;
		verticalSpeed = 0.0f;
	}
}

void Robot::Draw(mat4 &view)
{
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	vector<Model*> v;
	v.push_back(&body);
	v.push_back(&right_arm);
	v.push_back(&left_arm);

	for (int i = 0; i < v.size(); i++)
	{
		glLoadMatrixf(value_ptr(view*v[i]->M));
		glVertexPointer(3, GL_FLOAT, 0,&(v[i]->vertices[0]));
		glColorPointer(3, GL_FLOAT, 0, &(v[i]->colors[0]));
		glNormalPointer(GL_FLOAT, 0, &(v[i]->normals[0]));
		glDrawArrays(GL_TRIANGLES, 0, v[i]->vertex_count);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);*/
	body.Draw(view);
	left_arm.Draw(view);
	right_arm.Draw(view);
	ball.Draw(view);
	eyes.Draw(view);
}

void Robot::translateWhole(vec3 v)
{
	body.M = translate(body.M, v);
	right_arm.M = translate(right_arm.M, v);
	left_arm.M = translate(left_arm.M, v);
	
	/*if (isTurnRight & v.x!=0)
	{
		ball.M = rotate(ball.M, 2.0f, vec3(0, 1, 0));
		
	}
	else if (v.x!=0)
	{
		ball.M = rotate(ball.M, -2.0f, vec3(0, 1, 0));
	}*/
	ball.M = translate(ball.M, v);
	eyes.M = translate(eyes.M, v);
}

