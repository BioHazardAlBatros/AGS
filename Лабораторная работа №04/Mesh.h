#pragma once
#include "Data.h"

struct Vertex
{
	// �������������� ����������
	GLfloat coord[3];
	// ������ �������
	GLfloat normal[3];
	// ���������� ���������� �������� ����������� �����
	GLfloat texCoord[2];
};
class Mesh
{
public:
	// �����������
	Mesh();
	// �������� ���� �� �������� obj-�����
	bool load(std::string filename);
	// ����� ����
	void drawOne();
private:
	// ������ VAO-�������
	GLuint vao;
	// ������ VBO-������ ������
	GLuint vertexBuffer;
	// ������ VBO-������ ��������
	GLuint indexBuffer;
	// ���������� ������ � ����
	int vertexCount;
};	