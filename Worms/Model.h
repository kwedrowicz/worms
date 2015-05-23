#pragma once
#include <vector>

class Model
{
public:
	std::vector<float> vertices, colors, normals;
	int vertex_count = 0;
	bool loadObj(std::string path);
	Model();
	~Model();
};

