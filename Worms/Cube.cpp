#include "Cube.h"
#include <fstream>
#include <string>

using namespace std;



Cube::Cube()
{
	M = mat4(1);
	broken = 0;
	visible = 0;
	material = 0;
	for (int i = 0; i < 8; i++){
		vindex[i] = -1;
	}
}


Cube::~Cube()
{

}
