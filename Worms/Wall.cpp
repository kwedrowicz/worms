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


Wall::Wall(int sectorsizein, int sizex, int sizey, int sizez){
	sectorsize = sectorsizein;
	for (int i = 0; i < 3; i++)
	{
		ambient[i] = 0.2;
		specular[i] = 0;
		diffuse[i] = 0;
	}
	ambient[3] = 1; specular[3] = 1; diffuse[3] = 1;
	shininess = 0;

	M = mat4(1);
	xnum = sizex*sectorsize;
	ynum = sizey*sectorsize;
	znum = sizez*sectorsize;
	for (int i = 0; i < xnum; i++){
		cubes.push_back(std::vector < std::vector<Cube> >());
		for (int j = 0; j < ynum; j++){
			cubes[i].push_back(std::vector<Cube>());
			for (int k = 0; k < znum; k++){
				cubes[i][j].push_back(Cube());
			}
		}
	}

	int startx = -1 * xnum / 2;
	int starty = -1 * ynum / 2;
	int startz = -1 * znum / 2;

	for (int i = 0; i < sizex; i++){
		meshVertices.push_back(std::vector < std::vector<float> >());
		meshNormals.push_back(std::vector < std::vector<float> >());
		meshColors.push_back(std::vector < std::vector<float> >());
		meshIndices.push_back(std::vector < std::vector<int> >());
		currVIndex.push_back(std::vector <int>());
		indicesNumber.push_back(std::vector <int>());
		for (int j = 0; j < sizey; j++){
			meshVertices[i].push_back( std::vector<float>());
			meshNormals[i].push_back(std::vector<float>());
			meshIndices[i].push_back(std::vector<int>());
			meshColors[i].push_back(std::vector<float>());
			currVIndex[i].push_back(-1);
			indicesNumber[i].push_back(0);
		}
	}
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

void Wall::BlowCylinder(vec4 &myPosition, float r)
{
	vec4 pos = myPosition;
	mat4 invM = inverse(M);
	//cout << "pos before mult:" << pos.x<<" " << pos.y<<" " << pos.z << "\n";
	pos = invM * pos;
	int x = roundd(pos.x) + xnum / 2.0f;
	int y = roundd(pos.y) + ynum / 2.0f + 68;


	int istart = -1 * r;
	int jstart = -1 * r;
	int newi;
	int newj;
	for (int i = istart; i < r; i++){
		for (int j = jstart; j < r; j++){
			if (sqrt(pow(i, 2) + pow(j, 2)) < r){
				newi = i  + x;
				newj = j  + y;
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
	int startx, starty, endx, endy;
	startx = floor((x - r) / (float)sectorsize);
	if (startx < 0) startx = 0;
	starty = floor((y - r) / (float)sectorsize);
	if (starty < 0) starty = 0;
	endx = ceil((x + r) / (float)sectorsize);
	if (endx > xnum / 12) endx = xnum / sectorsize;
	endy = ceil((y + r) / (float)sectorsize);
	if (endy > ynum / 12) endy = ynum / sectorsize;
	for (int i = startx; i < endx; i++){
		for (int j = starty; j < endy; j++){
			CreateMesh(0, 0, 0, i, j);
		}
	}
	
}

//searches if the vertex was already indexed, if not indexes it; vx etc - direction of verticle
int Wall::FetchMeshVertexIndex(bool justnormals, int cubex, int cubey, int cubez, int vx, int vy, int vz, int blockx, int blocky){
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
	if (!justnormals){
		currVIndex[blockx][blocky]++;
		cubes[cubex][cubey][cubez].vindex[vx + 2 * vz + 4 * vy] = currVIndex[blockx][blocky];
		return -1 * (currVIndex[blockx][blocky] + 1);
	}
	else return -1;
}


void Wall::MeshPushSide(bool justnormals, int cubex, int cubey, int cubez, int side, int blockx, int blocky){
	vector<int> V; //indeks w tablicy wierzcho³ków
	vector<float> xV; //pozycje wierzcho³ka w szeœcianie
	vector<float> yV;
	vector<float> zV;
	if (side == 0 || side == 1 || side == 3){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 0, 1, 0, blockx, blocky));
		xV.push_back(-1);
		yV.push_back(1);
		zV.push_back(-1);
	}
	if (side == 0 || side == 3 || side == 4){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 1, 1, 0, blockx, blocky));
		xV.push_back(1);
		yV.push_back(1);
		zV.push_back(-1);
	}
	if (side == 0 || side == 1 || side == 5){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 0, 1, 1, blockx, blocky));
		xV.push_back(-1);
		yV.push_back(1);
		zV.push_back(1);
	}
	if (side == 0 || side == 4 || side == 5){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 1, 1, 1, blockx, blocky));
		xV.push_back(1);
		yV.push_back(1);
		zV.push_back(1);
	}
	if (side == 2 || side == 1 || side == 3){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 0, 0, 0, blockx, blocky));
		xV.push_back(-1);
		yV.push_back(-1);
		zV.push_back(-1);
	}
	if (side == 2 || side == 3 || side == 4){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 1, 0, 0, blockx, blocky));
		xV.push_back(1);
		yV.push_back(-1);
		zV.push_back(-1);
	}
	if (side == 2 || side == 1 || side == 5){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 0, 0, 1, blockx, blocky));
		xV.push_back(-1);
		yV.push_back(-1);
		zV.push_back(1);
	}
	if (side == 2 || side == 4 || side == 5){
		V.push_back(FetchMeshVertexIndex(justnormals, cubex, cubey, cubez, 1, 0, 1, blockx, blocky));
		xV.push_back(1);
		yV.push_back(-1);
		zV.push_back(1);
	}

	for (int i = 0; i<4; i++){
		if (V[i]<0){
			if (!justnormals){
				meshVertices[blockx][blocky].push_back(cubex + xV[i] / 2.0f - xnum / 2.0f);
				meshVertices[blockx][blocky].push_back(cubey + yV[i] / 2.0f - ynum / 2.0f);
				meshVertices[blockx][blocky].push_back(cubez + zV[i] / 2.0f - znum / 2.0f);
				meshNormals[blockx][blocky].push_back(xV[i]);
				meshNormals[blockx][blocky].push_back(yV[i]);
				meshNormals[blockx][blocky].push_back(zV[i]);
				meshColors[blockx][blocky].push_back(materials[cubes[cubex][cubey][cubez].material].r);
				meshColors[blockx][blocky].push_back(materials[cubes[cubex][cubey][cubez].material].g);
				meshColors[blockx][blocky].push_back(materials[cubes[cubex][cubey][cubez].material].b);
				V[i] = -1 * (V[i] + 1);
			}
			
		}
		else{
			
			meshNormals[blockx][blocky][V[i] * 3] += xV[i];
			meshNormals[blockx][blocky][V[i] * 3 + 1] += yV[i];
			meshNormals[blockx][blocky][V[i] * 3 + 2] += zV[i];
			meshColors[blockx][blocky][V[i] * 3] += materials[cubes[cubex][cubey][cubez].material].r;
			meshColors[blockx][blocky][V[i] * 3 + 1] += materials[cubes[cubex][cubey][cubez].material].g;
			meshColors[blockx][blocky][V[i] * 3 + 2] += materials[cubes[cubex][cubey][cubez].material].b;
		}
		//std::cout << V[i] << "; "<<cubex<<", " <<cubey <<", "<< cubez << endl;
	}
	if (!justnormals){
		meshIndices[blockx][blocky].push_back(V[3]);
		meshIndices[blockx][blocky].push_back(V[1]);
		meshIndices[blockx][blocky].push_back(V[0]);
		meshIndices[blockx][blocky].push_back(V[0]);
		meshIndices[blockx][blocky].push_back(V[2]);
		meshIndices[blockx][blocky].push_back(V[3]);
		indicesNumber[blockx][blocky] += 6;
	}
	/*if (cubex > 0 && cubex<xnum - 1 && cubez == 0 && cubey>0 && cubey < ynum - 1){
		for (int i = 0; i < 4; i++){
			std::cout << meshNormals[V[i] * 3] << ", " << meshNormals[V[i] * 3 + 1] << ", " << meshNormals[V[i] * 3 + 2] << ", " << endl;
		}
	}*/
}




//xdir ==0: rendering both left and right <0 rendering only left >0 rendering only right etc
void Wall::CreateMesh(int xdir, int ydir, int zdir, int blockx, int blocky){
	currVIndex[blockx][blocky] = -1;
	indicesNumber[blockx][blocky] = 0;
	meshIndices[blockx][blocky].clear();
	meshColors[blockx][blocky].clear();
	meshNormals[blockx][blocky].clear();
	meshVertices[blockx][blocky].clear();


	int startx, endx, starty, endy;
	startx = blockx * sectorsize - 2;
	if (startx<0) startx = 0;
	starty = blocky * sectorsize - 2;
	if (starty<0) starty = 0;
	endx = blockx * sectorsize + sectorsize + 2;
	if (endx>xnum)endx = xnum;
	endy = blocky * sectorsize + sectorsize + 2;
	if (endy>ynum)endy = ynum;

	for (int i = startx; i < endx; i++){
		for (int j = starty; j < endy; j++){
			for (int k = 0; k < znum; k++){
				for (int n = 0; n < 8; n++){
					cubes[i][j][k].vindex[n] = -1;
				}
			}
		}
	}

	startx = blockx * sectorsize - 1;
	if (startx<0) startx = 0;
	starty = blocky * sectorsize - 1;
	if (starty<0) starty = 0;
	endx = blockx * sectorsize + sectorsize + 1;
	if (endx>xnum)endx = xnum;
	endy = blocky * sectorsize + sectorsize + 1;
	if (endy>ynum)endy = ynum;

	for (int i = startx; i<endx; i++){
		for (int j = starty; j<endy; j++){
			for (int k = 0; k<znum; k++){
				if (cubes[i][j][k].visible && !cubes[i][j][k].broken){
					//cout << i << " " << j << " " << k << " " << endl;
					if (xdir <= 0 && (i - 1<0 || cubes[i - 1][j][k].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 1, blockx, blocky);
						else MeshPushSide(0, i, j, k, 1, blockx, blocky);
					}
					if (xdir >= 0 && (i + 1 >= xnum || cubes[i + 1][j][k].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 4, blockx, blocky);
						else MeshPushSide(0, i, j, k, 4, blockx, blocky);
					}
					if (ydir <= 0 && (j - 1<0 || cubes[i][j - 1][k].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 2, blockx, blocky);
						else MeshPushSide(0, i, j, k, 2, blockx, blocky);
					}
					if (ydir >= 0 && (j + 1 >= ynum || cubes[i][j + 1][k].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 0, blockx, blocky);
						else MeshPushSide(0, i, j, k, 0, blockx, blocky);
					}
					if (zdir <= 0 && (k - 1<0 || cubes[i][j][k - 1].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 3, blockx, blocky);
						else MeshPushSide(0, i, j, k, 3, blockx, blocky);
					}
					if (zdir >= 0 && (k + 1 >= znum || cubes[i][j][k + 1].broken)){
						if (i == blockx * sectorsize + sectorsize || j == blocky * sectorsize + sectorsize) MeshPushSide(1, i, j, k, 5, blockx, blocky);
						else MeshPushSide(0, i, j, k, 5, blockx, blocky);
					}
				}
			}
		}
	}
	for (int i = 0; i < meshColors[blockx][blocky].size(); i++){
		meshColors[blockx][blocky][i] /= 4;
	}
	std::cout << blockx << "; " << blocky << endl;
	std::cout << meshIndices[blockx][blocky].size() << endl;
	std::cout << meshVertices[blockx][blocky].size() << endl;
	std::cout << meshNormals[blockx][blocky].size() << endl;
	std::cout << meshColors[blockx][blocky].size() << endl;
}


void Wall::DrawMesh(mat4 &V){
	for (int i = 0; i < xnum / sectorsize; i++){
		for (int j = 0; j < ynum / sectorsize; j++){
			if (meshVertices[i][j].size()>0){
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);

				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);


				glVertexPointer(3, GL_FLOAT, 0, &meshVertices[i][j][0]);
				glColorPointer(3, GL_FLOAT, 0, &meshColors[i][j][0]);
				glNormalPointer(GL_FLOAT, 0, &meshNormals[i][j][0]);
				glLoadMatrixf(value_ptr(V*M));

				glDrawElements(GL_TRIANGLES, indicesNumber[i][j], GL_UNSIGNED_INT, &meshIndices[i][j][0]);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
			}
		}
	}

}

float Wall::HowFarFromSurface(vec4 &myPosition){
	vec4 pos = myPosition;
	mat4 invM = inverse(M);
	//cout << "pos before mult:" << pos.x<<" " << pos.y<<" " << pos.z << "\n";
	pos = invM * pos;
	
	float yyy = pos.y + (int)(ynum/2.0f)+70;
	int x = roundd(pos.x) + xnum/2.0f;
	int y = roundd(pos.y) + ynum/2.0f +70;
	int z = roundd(pos.z) + znum/2.0f;
	//cout << "pos after all:" << x << " " << y << " " << z << "\n";
	float yover = 0;

	if (x >= 0 && z >= 0 && x < xnum && z<znum && y>=0){
		if (y >= ynum){
			y = ynum - 1;
		}
		yover = yyy - y;
		if (!cubes[x][y][z].broken){
			for (int i = y; i < ynum; i++){
				if (cubes[x][i][z].broken){
					return y - i + yover -2;
				}
			}
			return y - ynum + yover - 2;
		}
		else {
			for (int i = y; i >= 0; i--){
				if (!cubes[x][i][z].broken){
					return y - i + yover -2;
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