#include "Shader.h"
#include "drawBox.h"
#include "Camera.h"
#include "GraphicObject.h"


LARGE_INTEGER startCounter, frequency;
Camera camera;
Shader shader;
std::vector<GraphicObject> graphicObjects;

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

void drawObject();

void display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader.activate();
	glm::mat4 viewMatrix = camera.getViewMatrix();
	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
	for (auto& graphObj : graphicObjects) 
	{
		shader.setUniform("modelViewMatrix", viewMatrix * graphObj.getModelMatrix());
		shader.setUniform("color", graphObj.getColor());
		drawBox();
	}

	glutSwapBuffers();

	char temp[80];
	int FPS = getFPS();
	sprintf_s(temp, "Lab3 FPS[%d]", FPS);
	glutSetWindowTitle(temp);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	camera.setProjectionMatrix(35.0, (float)w / h, 1.0, 500.0);
}

void mouseZoom(int wheel, int dir, int x, int y)
{
	//float delta = 2*dir;
	float delta = 0.5f*dir;
	printf("Zooming %s\n",(dir==1)?"in":"out");
	camera.zoom(delta);
}

void mouseMovement()
{
	static POINT oldPos ={0,0};
		   POINT newPos;
	static bool wasHeld = false;
		   bool isHeld = GetAsyncKeyState(VK_RBUTTON);
	GetCursorPos(&newPos);
	
	if (isHeld && wasHeld)
		camera.rotate(newPos.x-oldPos.x,oldPos.y-newPos.y);
	wasHeld = isHeld;
	oldPos = newPos;
	return;
}

void simulation()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	static double prevTime = startCounter.QuadPart;
		   double deltaTime = (currTime.QuadPart - prevTime) / frequency.QuadPart;
	prevTime = currTime.QuadPart;

	mouseMovement();
	float speedX = (GetAsyncKeyState(VK_DOWN) + (GetAsyncKeyState(VK_UP)) * (-1)) * 0.0005f * deltaTime, 
		  speedZ = (GetAsyncKeyState(VK_LEFT) + (GetAsyncKeyState(VK_RIGHT)) * (-1)) * 0.0002f * deltaTime;
	camera.moveOXZ(speedX,speedZ);
	
	glutPostRedisplay();
};

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("laba_03");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	shader.load(R"(assets\shaders\VertexShader.vsh)", R"(assets\shaders\FragmentShader.fsh)");
	
	//creating objects
	for (int i = 0; i < 12; i++)
	{
		//circle
		graphicObjects.emplace_back();
		graphicObjects.back().setPosition({3 * cos((3.1415 / 6.0) * i), 0, 3 * sin((3.1415 / 6.0) * i)});
		graphicObjects.back().setAngle(14.0f * i);
		graphicObjects.back().setColor({ 1.0/(i+1),0 ,0,1});
	}

	graphicObjects.emplace_back();
	graphicObjects.emplace_back();
	graphicObjects.back().setPosition({0.0f,1.0f,0.0f});
	graphicObjects.emplace_back();
	graphicObjects.back().setPosition({ 1.5f,0.0f,0.0f });
	graphicObjects.back().setAngle(45.0f);
	graphicObjects.back().setColor({ 0.0f,0.7f,0.0f,1.0f });
	//

	QueryPerformanceCounter(&startCounter);
	QueryPerformanceFrequency(&frequency);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseWheelFunc(mouseZoom);
	glutIdleFunc(simulation);
	glutMainLoop();
	return;
}

// функция для вывода квадрата с ребрами равными единице (от -0.5 до +0.5)
void drawObject()
{
	// переменные для вывода объекта (прямоугольника из двух треугольников)
	static bool init = true;
	static GLuint VAO_Index = 0;		// индекс VAO-буфера
	static GLuint VBO_Index = 0;		// индекс VBO-буфера
	static int VertexCount = 0;			// количество вершин

	// при первом вызове инициализируем VBO и VAO
	if (init) {
		init = false;
		// создание и заполнение VBO
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

		// создание VAO
		glGenVertexArrays(1, &VAO_Index);
		glBindVertexArray(VAO_Index);
		// заполнение VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		// "отвязка" буфера VAO, чтоб случайно не испортить
		glBindVertexArray(0);

		// указание количество вершин
		VertexCount = 6;
	}

	// выводим прямоугольник
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}