#include "Robot.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <cstdio>
#include <cmath>
#define M_PI 3.14159265358979323846

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
	if (isShooting)
	{
		missile.Draw(view, M);
	}
	else
	{
		missile.Draw(view, M*right_arm.M);
	}
}

void Robot::Shot()
{
	missile.M = mat4(1.0);
	missile_speed = 15.0f;
	arm_angle -= (M_PI / 2);
	isShooting = true;
}

void Robot::calculateShot(int time)
{
	cout << "Kat: " << arm_angle * 180 / M_PI << endl;
	//system("pause");
	horizontalSpeed = cos(arm_angle)*missile_speed;
	cout << "Hor: " << horizontalSpeed << endl;
	verticalSpeed = sin(arm_angle)*missile_speed;
	cout << "Ver: " << verticalSpeed << endl;

	verticalSpeed += gravity * time / 1000.0f;
	cout << "Ver: " << verticalSpeed << endl;
	float new_arm_angle = atan(verticalSpeed / horizontalSpeed);
	missile_speed = horizontalSpeed / cos(new_arm_angle);
	//missile.M = rotate(missile.M, new_arm_angle - arm_angle, vec3(0.0f, 0.0f, 1.0f));
	arm_angle = new_arm_angle;
	//missile.M = mat4(1.0);

	missile.M = translate(missile.M, vec3(horizontalSpeed*time / 1000.0f, verticalSpeed*time / 1000.0f, 0.0f));
	//missile.M = translate(missile.M, vec3(0.0f, 0.1f, 0.0f));

	/*missile.M = translate(missile.M, vec3(0, missile_translate, 0));
	missile.M = rotate(missile.M, arm_angle, vec3(0.0f, 0.0f, 1.0f));
	missile.M = translate(missile.M, vec3(0, -1*missile_translate, 0));

	missile_translate += (missile_speed*time / 1000.0f);*/


}

