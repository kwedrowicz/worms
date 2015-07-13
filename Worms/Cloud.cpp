#include "Cloud.h"

using namespace glm;
Cloud::Cloud()
{
	radius = (rand() % 40) / 100.0 + 0.2;  // 0.2 - 0.6
	scaleX = 1.5 + ((rand() % 100) / 100.0);  // 1.5 - 2.5
	positionY = 3 + ((rand() % 300) / 100.0);  // od 3 do 6
	positionX = ((rand() % 100) / 10.0) - 5;  // od -5 do 5
	positionZ = ((rand() % 200) / 100.0) - 1;   // od 0 do 2
	speed = (5 + (rand() % 40)) / 10.0;  // ???
}

void Cloud::Draw(glm::mat4 &V)
{
	if (positionX > 20) positionX = -20;
	float ambient[4] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	//glColor3d(0.5f, 0.95f, 0.75f);
	glColor3d(0.65f, 1.0f, 1.0f);
	M = mat4(1.0f);
	M = translate(M, vec3(positionX, positionY, 0));
	M = scale(M, vec3(scaleX, 1, 1));
	glLoadMatrixf(value_ptr(V*M));
	glutSolidSphere(radius, 20, 20);
	glEnable(GL_TEXTURE_2D);
	glColor3d(1, 1, 1);
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