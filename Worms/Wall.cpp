#include "Wall.h"
#include "Cube.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <iostream>
#include <time.h> 

using namespace std;
using namespace glm;

double roundd(double d)
{
	return floor(d + 0.5);
}

float roundd(float d)
{
	return floor(d + 0.5);
}


Wall::Wall(int sizex, int sizey, int sizez){
	currVIndex = -1;
	indicesNumber = 0;

	for (int i = 0; i < 3; i++)
	{
		ambient[i] = 0.2;
		specular[i] = 0;
		diffuse[i] = 0;
	}
	ambient[3] = 1; specular[3] = 1; diffuse[3] = 1;
	shininess = 0;

	M = mat4(1);
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

	
}



Wall::~Wall()
{
	
}


void Wall::GrassyCuboid(){
	materials.push_back(Material(1, 0.5, 0));
	materials.push_back(Material(0, 1, 0));
	for (int i = 0; i < xnum; i++){
		for (int j = 0; j < ynum; j++){
			for (int k = 0; k < znum; k++){
				cubes[i][j][k].broken = 0;
				//cubes[i][j][k].M = translate(cubes[i][j][k].M, vec3(startx, starty, startz));
				//cubes[i][j][k].M = translate(cubes[i][j][k].M, vec3(i, j, k));
				if (i == 0 || i == xnum - 1 || j == 0 || j == ynum - 1 || k == 0 || k == znum - 1){
					cubes[i][j][k].visible = 1;
					cubes[i][j][k].material = 1;
				}
				else{
				}
			}
		}
	}
}


void Wall::AddCube(int x, int y, int z, int mat){
	cubes[x][y][z].material = mat;
	cubes[x][y][z].visible = 1;
	cubes[x][y][z].broken = 0;
}


bool Roll(float probability){
	if (probability >= (float)rand() / (float)RAND_MAX) {
		return 1;
	}
	else{
		return 0;
	}
}


void Wall::LetTheEarthPutForth(){
	materials.push_back(Material(0, 1.35, 0));//mat 0 grass
	materials.push_back(Material(1.2, 0.6, 0));//mat 1 dirt
	materials.push_back(Material(0.6, 1.0, 1));//mat 2 stone
	materials.push_back(Material(1, 1.4, 0.8));//mat 3 wood
	materials.push_back(Material(0.6, 3, 0.6));//mat 4 leaf

	int NStartingPoints=10;
	int StartingPointsReach = ynum / 10;

	int GrowthDistance = 2;
	float GrowthProbality = 0.06;

	//int StoneReachMin = ynum / 2;
	//int StoneReachMax = ynum / 1.8;

	int GrassThickness = ceil(ynum / 20);
	int NGrassMounds = 3;

	int TreeMinThickness = ceil(znum / 15.0);
	int TreeMaxThickness = TreeMinThickness * 1.5;
	float TreeCrownThickness = 1.5;
	int TreeMaxHeight = TreeMaxThickness * (6 + ceil(TreeCrownThickness) + 0.5);
	int NTrees = 5;


	srand(time(NULL));

	for (int i = 0; i < NStartingPoints; i++){
		int xp = rand() % (xnum/2) + xnum/4;
		int zp = rand() % (znum / 2) + znum/4;
		int yp = rand() % StartingPointsReach;
		AddCube(xp, yp, zp, 2);
	}
	int matt=2;
	for (int j = 1; j < ynum - TreeMaxHeight - GrassThickness; j++){
		if (j == ynum - TreeMaxHeight - GrassThickness - 1){ matt = 0; }
		for (int i = 0; i < xnum; i++){
			for (int k = 0; k < znum; k++){
				if (cubes[i][j - 1][k].visible){
					AddCube(i, j, k, matt);
				}
				else{
					for (int x = -1 * GrowthDistance; x <= GrowthDistance; x++){
						for (int z = -1 * GrowthDistance; z <= GrowthDistance; z++){
							if (i + x >= 0 && i + x < xnum && k + z >= 0 && k + z < znum){
								if (x != 0 || k != 0){
									if (cubes[i + x][j - 1][k + z].visible){
										if (Roll(GrowthProbality / (abs(x)*abs(x) + abs(z)*abs(z)))){
											AddCube(i, j, k, matt);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (int n = 0; n < NGrassMounds; n++){
		int xp = rand() % (xnum / 2) + xnum / 4;
		int zp = rand() % (znum / 2) + znum / 4;
		int yp = ynum - TreeMaxHeight - GrassThickness;
	//	int height = 0.7
	}
	
	for (int n = 0; n < NTrees; n++){
		
		int xp = (rand() % (int)(xnum * 4 / 6 / NTrees)) + (xnum / 6) + (xnum * 4 * n  / 6 / NTrees);
		int yp = ynum - TreeMaxHeight;
		int zp = znum / 2;
		int thick = TreeMinThickness + rand() % (TreeMaxThickness - TreeMinThickness +1);
		int high = thick * (3 + (rand() % 4));

		bool basefound = 0;
		while (!basefound && yp>1){
			yp--;
			basefound = 1;
			for (int i = -1 * thick / 2; i < -1 * thick / 2 + thick; i++){
				for (int k = -1 * thick / 2; k < -1 * thick / 2 + thick; k++){
					if (! (cubes[xp + i][yp - 1][zp + k].visible)){
						basefound = 0;
					}
				}
			}
		}

		if (cubes[xp][yp - 1][zp].visible){
			for (int i = -1 * thick / 2; i < -1 * thick / 2 + thick; i++){
				for (int k = -1 * thick / 2; k < -1 * thick / 2 + thick; k++){
					for (int j = 0; j < high; j++){
						//if (xp + i>0 && xp + i<xnum && yp + j>0 && yp + j<ynum && zp + k>0 && zp + k<znum)
						AddCube(xp + i, yp + j, zp + k, 3);
					}
				}
			}
			for (int i = -1 * thick * TreeCrownThickness ; i < thick * TreeCrownThickness; i++){
				for (int j = -1 * thick * TreeCrownThickness; j < thick * TreeCrownThickness; j++){
					for (int k = -1 * thick * TreeCrownThickness; k < thick * TreeCrownThickness; k++){
						if (pow(i,2) + pow(j,2) + pow(k,2) < pow(thick*TreeCrownThickness,2)){
							AddCube(xp + i, yp + j + high, zp + k, 4);
						}
					}
				}
			}
		}

	}
	
	
}

/*
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
*/

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

//searches if the vertex was already indexed, if not indexes it; vx etc - direction of verticle
int Wall::FetchMeshVertexIndex(int cubex, int cubey, int cubez, int vx, int vy, int vz){
	/*if (vx>0){
	if (cubex+1<numx && cube[cubex+1][cubey][cubez].vertexIndex[vz*2+vy*4]>-1){
	return cube[cubex+1][cubey][cubez].vertexIndex[vz*2+vy*4];
	}
	} */
	if (cubes[cubex][cubey][cubez].vindex[vx + (2 * vz) + (4 * vy)]>-1){
		return cubes[cubex][cubey][cubez].vindex[vx + (2 * vz) + (4 * vy)];
	}
	if (vx <= 0){
		if (cubex - 1 >= 0){
			if (cubes[cubex - 1][cubey][cubez].vindex[vz * 2 + (vy * 4) + 1] > -1){
				cubes[cubex][cubey][cubez].vindex[vx + 2 * vz + 4 * vy] = cubes[cubex - 1][cubey][cubez].vindex[vz * 2 + (vy * 4) + 1];
				return cubes[cubex - 1][cubey][cubez].vindex[vz * 2 + (vy * 4) + 1];
			}
		}
	}
	/*if (vy>0){
	if (cubey+1<numy && cube[cubex][cubey+1][cubez].vertexIndex[vx+vz*2]>-1){
	return cube[cubex][cubey+1][cubez].vertexIndex[vx+vz*2];
	}
	}*/
	if (vy<=0){
		if (cubey - 1 >= 0){
			if (cubes[cubex][cubey - 1][cubez].vindex[vx + (vz * 2) + 4] > -1){
				cubes[cubex][cubey][cubez].vindex[vx + 2 * vz + 4 * vy] = cubes[cubex][cubey - 1][cubez].vindex[vx + (vz * 2) + 4];
				return cubes[cubex][cubey - 1][cubez].vindex[vx + (vz * 2) + 4];
			}
		}
	}

	if (vz<=0){
		if (cubez - 1 >= 0){
			if (cubes[cubex][cubey][cubez - 1].vindex[vx + 2 + (vy * 4)] > -1){
				cubes[cubex][cubey][cubez].vindex[vx + 2 * vz + 4 * vy] = cubes[cubex][cubey][cubez - 1].vindex[vx + 2 + (4 * vy)];
				return cubes[cubex][cubey][cubez-1].vindex[vx + 2 + (4*vy)];
			}
		}
	}
	currVIndex++;
	cubes[cubex][cubey][cubez].vindex[vx + 2 * vz + 4 * vy] = currVIndex;
	return -1 * (currVIndex + 1);
}


void Wall::MeshPushSide(int cubex, int cubey, int cubez, int side){
	vector<int> V; //indeks w tablicy wierzcho³ków
	vector<float> xV; //pozycje wierzcho³ka w szeœcianie
	vector<float> yV;
	vector<float> zV;
	if (side == 0 || side == 1 || side == 3){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 0, 1, 0));
		xV.push_back(-1);
		yV.push_back(1);
		zV.push_back(-1);
	}
	if (side == 0 || side == 3 || side == 4){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 1, 1, 0));
		xV.push_back(1);
		yV.push_back(1);
		zV.push_back(-1);
	}
	if (side == 0 || side == 1 || side == 5){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 0, 1, 1));
		xV.push_back(-1);
		yV.push_back(1);
		zV.push_back(1);
	}
	if (side == 0 || side == 4 || side == 5){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 1, 1, 1));
		xV.push_back(1);
		yV.push_back(1);
		zV.push_back(1);
	}
	if (side == 2 || side == 1 || side == 3){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 0, 0, 0));
		xV.push_back(-1);
		yV.push_back(-1);
		zV.push_back(-1);
	}
	if (side == 2 || side == 3 || side == 4){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 1, 0, 0));
		xV.push_back(1);
		yV.push_back(-1);
		zV.push_back(-1);
	}
	if (side == 2 || side == 1 || side == 5){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 0, 0, 1));
		xV.push_back(-1);
		yV.push_back(-1);
		zV.push_back(1);
	}
	if (side == 2 || side == 4 || side == 5){
		V.push_back(FetchMeshVertexIndex(cubex, cubey, cubez, 1, 0, 1));
		xV.push_back(1);
		yV.push_back(-1);
		zV.push_back(1);
	}

	for (int i = 0; i<4; i++){
		if (V[i]<0){
			meshVertices.push_back(cubex + xV[i] / 2.0f - xnum / 2.0f);
			meshVertices.push_back(cubey + yV[i] / 2.0f - ynum / 2.0f);
			meshVertices.push_back(cubez + zV[i] / 2.0f - znum / 2.0f);
			meshNormals.push_back(xV[i]);
			meshNormals.push_back(yV[i]);
			meshNormals.push_back(zV[i]);
			meshColors.push_back(materials[cubes[cubex][cubey][cubez].material].r);
			meshColors.push_back(materials[cubes[cubex][cubey][cubez].material].g);
			meshColors.push_back(materials[cubes[cubex][cubey][cubez].material].b);
			V[i] = -1 * (V[i] + 1);
			
		}
		else{
			
			meshNormals[V[i]*3] += xV[i];
			meshNormals[V[i]*3+1] += yV[i];
			meshNormals[V[i]*3+2] += zV[i];
			meshColors[V[i]*3] += materials[cubes[cubex][cubey][cubez].material].r;
			meshColors[V[i]*3+1] += materials[cubes[cubex][cubey][cubez].material].g;
			meshColors[V[i]*3+2] += materials[cubes[cubex][cubey][cubez].material].b;
		}
		//std::cout << V[i] << "; "<<cubex<<", " <<cubey <<", "<< cubez << endl;
	}

	meshIndices.push_back(V[3]);
	meshIndices.push_back(V[1]);
	meshIndices.push_back(V[0]);
	meshIndices.push_back(V[0]);
	meshIndices.push_back(V[2]);
	meshIndices.push_back(V[3]);
	indicesNumber += 6;
	/*if (cubex > 0 && cubex<xnum - 1 && cubez == 0 && cubey>0 && cubey < ynum - 1){
		for (int i = 0; i < 4; i++){
			std::cout << meshNormals[V[i] * 3] << ", " << meshNormals[V[i] * 3 + 1] << ", " << meshNormals[V[i] * 3 + 2] << ", " << endl;
		}
	}*/
}




//xdir ==0: rendering both left and right <0 rendering only left >0 rendering only right etc
void Wall::CreateMesh(int xdir, int ydir, int zdir){
	currVIndex = -1;
	indicesNumber = 0;
	meshIndices.clear();
	meshColors.clear();
	meshNormals.clear();
	meshVertices.clear();


	for (int i = 0; i<xnum; i++){
		for (int j = 0; j<ynum; j++){
			for (int k = 0; k<znum; k++){
				if (cubes[i][j][k].visible && !cubes[i][j][k].broken){
					//cout << i << " " << j << " " << k << " " << endl;
					if (xdir <= 0 && (i - 1<0 || cubes[i - 1][j][k].broken)){
						MeshPushSide(i, j, k, 1);
					}
					if (xdir >= 0 && (i + 1 >= xnum || cubes[i + 1][j][k].broken)){
						MeshPushSide(i, j, k, 4);
					}
					if (ydir <= 0 && (j - 1<0 || cubes[i][j - 1][k].broken)){
						MeshPushSide(i, j, k, 2);
					}
					if (ydir >= 0 && (j + 1 >= ynum || cubes[i][j + 1][k].broken)){
						MeshPushSide(i, j, k, 0);
					}
					if (zdir <= 0 && (k - 1<0 || cubes[i][j][k - 1].broken)){
						MeshPushSide(i, j, k, 3);
					}
					if (zdir >= 0 && (k + 1 >= znum || cubes[i][j][k + 1].broken)){
						MeshPushSide(i, j, k, 5);
					}
				}
			}
		}
	}
	for (int i = 0; i < meshColors.size(); i++){
		meshColors[i] /= 4;
	}
	//cout << indicesNumber << endl;
}


void Wall::DrawMesh(mat4 &V){

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);


	glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0]);
	glColorPointer(3, GL_FLOAT, 0, &meshColors[0]);
	glNormalPointer(GL_FLOAT, 0, &meshNormals[0]);
	glLoadMatrixf(value_ptr(V*M));

	glDrawElements(GL_TRIANGLES, indicesNumber, GL_UNSIGNED_INT, &meshIndices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}

float Wall::HowFarFromSurface(vec4 &myPosition){
	vec4 pos = myPosition;
	mat4 invM = inverse(M);
	//cout << "pos before mult:" << pos.x<<" " << pos.y<<" " << pos.z << "\n";
	pos = invM * pos;
	
	float yyy = pos.y + (int)(ynum)/2.0f;
	int x = roundd(pos.x) + xnum/2.0f;
	int y = roundd(pos.y) + ynum/2.0f +60;
	int z = roundd(pos.z) + znum/2.0f;
	//cout << "pos after all:" << x << " " << y << " " << z << "\n";
	int yover = 0;

	if (x >= 0 && z >= 0 && x < xnum && z<znum && y>=0){
		if (y >= ynum){
			yover = y - ynum - 1;
			y = ynum - 1;
		}
		if (!cubes[x][y][z].broken){
			for (int i = y; i < ynum; i++){
				if (cubes[x][i][z].broken){
					return y - i;
				}
			}
			return y -ynum;
		}
		else {
			for (int i = y; i >= 0; i--){
				if (!cubes[x][i][z].broken){
					return y - i;
				}
			}
			return 100000;
		}
	}
	else{
		return 200000;
	}
	return 0;
}