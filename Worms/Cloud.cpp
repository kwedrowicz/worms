#include "Cloud.h"

using namespace glm;
Cloud::Cloud()
{
	radius = (rand() % 40) / 100 + 0.2;
	scaleX = 1.5 + ((rand() % 100) / 100);
	positionY = 3 + ((rand() % 300) / 100);
	positionX = ((rand() % 100) / 10) - 5;
	speed = (rand() % 100) / 1000;
}

void Cloud::Draw(glm::mat4 &V)
{
	if (positionX > 20) positionX = -20;
	
	glColor3d(0.4f, 0.85f, 0.65f);
	M = mat4(1.0f);
	M = translate(M, vec3(positionX, positionY, 0));
	M = scale(M, vec3(scaleX, 1, 1));
	glLoadMatrixf(value_ptr(V*M));
	glutSolidSphere(radius, 20, 20);
}

/*GLfloat shininess = 0.5f;
float ambient[4], diffuse[4], specular[4];
for (int i = 0; i < 3; i++)
{
ambient[i] = (rand() % 50 + 50) / 100;
specular[i] = (rand() % 50 + 50) / 100;
diffuse[i] = (rand()%50 + 50)/100;
}
ambient[3] = 1; specular[3] = 1; diffuse[3] = 1;
glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);*/