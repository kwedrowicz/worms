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
	missile.loadObj("tex_missile.obj");
	eyes.shininess = 1.0f;
	for (int i = 0; i < 3; i++)
	{
		if (i != 1)
		{
			eyes.ambient[i] = 0.5;
			eyes.specular[i] = 1;
			eyes.diffuse[i] = 1;
		}
	}
	M = translate(M,vec3(0.0f, -3.5f, 0.0f));
}


Robot::~Robot()
{
}

void Robot::turnFaceSide()
{
	M = rotate(M, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
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
		M = translate(M, vec3(0.0f, verticalSpeed*time / 1000.0f, 0.0f));
	else
	{
		M = translate(M, vec3(0.0f, verticalSpeed*time*altitude_last / (1000.0f * abs(altitude_last - altitude)), 0.0f));
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
	body.Draw(view, M);
	left_arm.Draw(view,M*body.M);
	right_arm.Draw(view,M*body.M);
	ball.Draw(view,M);
	eyes.Draw(view,M);
	missile.Draw(view,M*right_arm.M);
}

