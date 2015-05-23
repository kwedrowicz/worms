#include "Model.h"
#include <fstream>
#include <string>

using namespace std;


Model::Model()
{
}


Model::~Model()
{
}

void Model::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glColorPointer(3, GL_FLOAT, 0, &colors[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

bool Model::loadObj(string path)
{
	vertices.clear();
	colors.clear();
	normals.clear();
	vertex_count = 0;
	fstream plik;
	plik.open(path, ios::in);
	if (plik.good() == false)
		return false;

	vector<float> local_normals;
	vector<float> local_vertices;
	vector<int> normalsIndices;
	vector<int> vertexIndices;
	string buf;
	float fbuf;
	while (!plik.eof())
	{
		plik >> buf;
		if (buf.compare("v") == 0)
		{
			plik >> fbuf;
			local_vertices.push_back(fbuf);
			plik >> fbuf;
			local_vertices.push_back(fbuf);
			plik >> fbuf;
			local_vertices.push_back(fbuf);
		}
		else if (buf.compare("vn") == 0)
		{
			plik >> fbuf;
			local_normals.push_back(fbuf);
			plik >> fbuf;
			local_normals.push_back(fbuf);
			plik >> fbuf;
			local_normals.push_back(fbuf);
		}
		else if (buf.compare("f") == 0)
		{
			int indexa, indexb;
			for (int i = 0; i < 3; i++)
			{
				plik >> buf;
				sscanf_s(buf.c_str(), "%d//%d", &indexa, &indexb);
				vertexIndices.push_back(indexa - 1);
				normalsIndices.push_back(indexb - 1);
			}
		}
	}
	for (int i = 0; i < normalsIndices.size(); i++)
	{
		int a = normalsIndices[i] * 3;
		int b = vertexIndices[i] * 3;
		vertices.push_back(local_vertices[b]);
		vertices.push_back(local_vertices[b + 1]);
		vertices.push_back(local_vertices[b + 2]);
		normals.push_back(local_normals[a]);
		normals.push_back(local_normals[a + 1]);
		normals.push_back(local_normals[a + 2]);
		colors.push_back((rand() % 50) / 100 + 0.25);
		colors.push_back((rand() % 50) / 100 + 0.5);
		colors.push_back((rand() % 50) / 100 + 0.1);
		vertex_count++;
	}
	plik.close();
	return true;
}
