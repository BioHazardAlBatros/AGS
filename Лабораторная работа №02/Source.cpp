#include "Shader.h"


// ������������ ������������ ���� (��� ��������)
using namespace glm;
using namespace std;

LARGE_INTEGER startCounter, frequency;


double getSimulationTime()
{
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return temp.QuadPart - startCounter.QuadPart;
}

int getFPS()
{
	static int frameCount = 0;
	//static int halfSecond = frequency.QuadPart / 2;
	static int FPS = 0;
	++frameCount;
	if (getSimulationTime() >= frequency.QuadPart)
	{
		QueryPerformanceCounter(&startCounter);
		FPS = frameCount;
		frameCount = 0;
	}
	return FPS;
}

// ������������ ������ (���� ������ ����)
Shader shader;
glm::vec2 offset = { 0.0,0 };

// ������� ��� ������ �������� � ������� ������� ������� (�� -0.5 �� +0.5)
void drawObject();

// ������� ���������� ��� ����������� ����
// � ��� ����� � �������������, �� ������� glutPostRedisplay
void display()
{
	// �������� ����� �����
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �������� ���� ������� (�� ������ ������)
	glEnable(GL_DEPTH_TEST);

	// ���������� ������
	shader.activate();
	shader.setUniform("offset", offset);

	shader.setUniform("color1", { 1.0,0.0,0.0,1.0 });
	shader.setUniform("color2", { 0.0,0.0,1.0,1.0 });

	// ������� ������������� 
	drawObject();

	// ����� ��������� � ������� �������
	glutSwapBuffers();

	char temp[80];
	int FPS = getFPS();
//	sprintf_s(temp, "FPS[%d] offset: %.3f, %.3f", FPS,offset.x,offset.y);
	sprintf_s(temp, "FPS[%d]", FPS);
	glutSetWindowTitle(temp);
}

// �������, ���������� ��� ��������� �������� ����
void reshape(int w, int h)
{
	// ���������� ����� ������� ���������, ������ ���� ������� ����
	glViewport(0, 0, w, h);
}

// ������� ���������� ����� ��������� �����������, �.�. ����������� �����

inline float changeDirection(float pos, float& speed, float deltaTime, float boundary)
{
	float newPos = pos + speed * deltaTime;
	if (newPos - boundary > 0)
		return -fabs(speed);
	if (newPos + boundary < 0)
		return fabs(speed);
	return speed;
}

void simulation()
{
//	assert(offset.x < 0.6);
	static double prevTime = startCounter.QuadPart;
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	double deltaTime = (currTime.QuadPart - prevTime) / frequency.QuadPart;
	prevTime = currTime.QuadPart;

	static glm::vec2 speed = { 1,0.5 };

	//if (offset.x + speed.x * deltaTime - 0.5 > 0) speed.x = -1;
	//if (offset.x + speed.x * deltaTime + 0.5 < 0) speed.x = 1;
	//if (offset.y + speed.y * deltaTime - 0.5 > 0) speed.y = -0.5;
	//if (offset.y + speed.y * deltaTime + 0.5 < 0) speed.y =  0.5;

	speed.x = changeDirection(offset.x, speed.x, deltaTime, 0.5);
	speed.y = changeDirection(offset.y, speed.y, deltaTime, 0.5);

	offset.x += speed.x * deltaTime;
	offset.y += speed.y * deltaTime;

	// ������������ ����
	glutPostRedisplay();
};

// �������� �������
void main(int argc, char** argv)
{
	// ������������� ���������� GLUT
	glutInit(&argc, argv);
	// ������������� ������� (������ ������)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// ���������� � ������ OpenGL (������ 3.3 ��� ��������� �������� �������������)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// ������������� ������� ����� ���� ����
	glutInitWindowPosition(300, 100);
	// ������������� ������ ����
	glutInitWindowSize(800, 600);
	// �������� ����
	glutCreateWindow("laba_02");

	// ������������� GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}

	// ����������� ������� ������ OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	// �������� �������
	if (!shader.load(R"(assets\shaders\Example.vsh)", R"(assets\shaders\Example.fsh)"))
		return;

	QueryPerformanceCounter(&startCounter);
	QueryPerformanceFrequency(&frequency);
	// ������������� �������, ������� ����� ���������� ��� ����������� ����
	glutDisplayFunc(display);
	// ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
	glutReshapeFunc(reshape);
	// ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
	glutIdleFunc(simulation);
	// �������� ���� ��������� ��������� ��
	glutMainLoop();
	return;
}

// ������� ��� ������ �������� � ������� ������� ������� (�� -0.5 �� +0.5)
void drawObject()
{
	// ���������� ��� ������ ������� (�������������� �� ���� �������������)
	static bool init = true;
	static GLuint VAO_Index = 0;		// ������ VAO-������
	static GLuint VBO_Index = 0;		// ������ VBO-������
	static int VertexCount = 0;			// ���������� ������

	// ��� ������ ������ �������������� VBO � VAO
	if (init) {
		init = false;
		// �������� � ���������� VBO
		glGenBuffers(1, &VBO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		GLfloat	Verteces[] = {
			-0.5,	+0.5,
			-0.5,	-0.5,
			+0.5,	+0.5,
			+0.5,	+0.5,
			-0.5,	-0.5,
			+0.5,	-0.5
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);

		// �������� VAO
		glGenVertexArrays(1, &VAO_Index);
		glBindVertexArray(VAO_Index);
		// ���������� VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		// "�������" ������ VAO, ���� �������� �� ���������
		glBindVertexArray(0);

		// �������� ���������� ������
		VertexCount = 6;
	}

	// ������� �������������
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}