#include "Wall.h"
#include "Cube.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;
using namespace glm;


Wall::Wall(int sizex, int sizey, int sizez){
	for (int i = 0; i < 3; i++)
	{
		ambient[i] = 0.2;
		specular[i] = 0;
		diffuse[i] = 0;
	}
	ambient[3] = 1; specular[3] = 1; diffuse[3] = 1;
	shininess = 0;

	M = mat4(1);
	materials.push_back(Material(1.2, 0.6, 0));
	materials.push_back(Material(0, 1.6, 0));
	xnum = sizex;
	ynum = sizey;
	znum = sizez;
	for (int i = 0; i < sizex; i++){
		cubes.push_back(std::vector < std::vector<Cube> >());
		for (int j = 0; j < sizey; j++){
			cubes[i].push_back(std::vector<Cube>());
			for (int k = 0; k < sizez; k++){
				cubes[i][j].push_back(Cube());
			}
		}
	}

	int startx = -1 * xnum / 2;
	int starty = -1 * ynum / 2;
	int startz = -1 * znum / 2;

	for (int i = 0; i < sizex; i++){
		for (int j = 0; j < sizey; j++){
			for (int k = 0; k < sizez; k++){
				cubes[i][j][k].M = translate(cubes[i][j][k].M, vec3(startx, starty, startz));
				cubes[i][j][k].M = translate(cubes[i][j][k].M, vec3(i, j, k));
				if (i == 0 || i == sizex - 1 || j == 0 || j == sizey - 1 || k == 0 || k == sizez - 1){
					cubes[i][j][k].visible=1;
					cubes[i][j][k].material = 1;
				}
				else{
				}
			}
		}
	}
}


Wall::~Wall()
{
	
}

void Wall::Draw(mat4 &V)
{
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	float step = 1;
	int material;
	mat4 temp=M;
	M = V*M;
	for (int i = 0; i < xnum; i++){
		for (int j = 0; j < ynum; j++){
			for (int k = 0; k < znum; k++){
				if (!cubes[i][j][k].broken && cubes[i][j][k].visible){
					material = cubes[i][j][k].material;
					materials[material].M = this->M*cubes[i][j][k].M;
					materials[material].Draw();
				}
			}
		}
	}
	M = temp;
}

void Wall::BlowCylinder(float x, float y, float r)
{
	int istart = -1 * r;
	int jstart = -1 * r;
	int newi;
	int newj;
	for (int i = istart; i < r; i++){
		for (int j = jstart; j < r; j++){
			if (sqrt(pow(i, 2) + pow(j, 2)) < r){
				newi = i + xnum / 2 +x;
				newj = j + ynum / 2 + y;
				if (newi>=0 && newi<xnum && newj>=0 && newj < ynum){
					for (int k = 0; k < znum; k++){
						cubes[newi][newj][k].broken = 1;
						if (newi - 1 >= 0) cubes[newi-1][newj][k].visible = 1;
						if (newi + 1 <xnum) cubes[newi+1][newj][k].visible = 1;
						if (newj - 1 >=0) cubes[newi][newj-1][k].visible = 1;
						if (newj + 1 <ynum) cubes[newi][newj + 1][k].visible = 1;
					}
				}
			}
		}
	}
}