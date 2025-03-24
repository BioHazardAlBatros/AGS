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
public:
	Mesh();
	bool load(std::string sourceFile);
	void drawOne();
private:
	GLuint vao;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	int vertexCount;
};	