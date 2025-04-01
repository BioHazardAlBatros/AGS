#pragma once
#include "Data.h"

struct Vertex
{
	GLfloat coord[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
};
class Mesh
{
	GLuint vao;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	int vertexCount;
public:
	Mesh();
	bool load(std::string sourceFile);
	void drawOne();
	void drawMany(int count);
};	