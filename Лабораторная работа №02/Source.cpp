#include "Shader.h"


// используемые пространства имен (для удобства)
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

// используемый шейдер (пока только один)
Shader shader;
glm::vec2 offset = { 0.0,0 };

// функция для вывода квадрата с ребрами равными единице (от -0.5 до +0.5)
void drawObject();

// функция вызывается при перерисовке окна
// в том числе и принудительно, по команде glutPostRedisplay
void display()
{
	// отчищаем буфер кадра
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// включаем тест глубины (на всякий случай)
	glEnable(GL_DEPTH_TEST);

	// активируем шейдер
	shader.activate();
	shader.setUniform("offset", offset);

	shader.setUniform("color1", { 1.0,0.0,0.0,1.0 });
	shader.setUniform("color2", { 0.0,0.0,1.0,1.0 });

	// выводим прямоугольник 
	drawObject();

	// смена переднего и заднего буферов
	glutSwapBuffers();

	char temp[80];
	int FPS = getFPS();
//	sprintf_s(temp, "FPS[%d] offset: %.3f, %.3f", FPS,offset.x,offset.y);
	sprintf_s(temp, "FPS[%d]", FPS);
	glutSetWindowTitle(temp);
}

// функция, вызываемая при изменении размеров окна
void reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, w, h);
}

// функция вызывается когда процессор простаивает, т.е. максимально часто

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

	// перерисовать окно
	glutPostRedisplay();
};

// основная функция
void main(int argc, char** argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300, 100);
	// устанавливаем размер окна
	glutInitWindowSize(800, 600);
	// создание окна
	glutCreateWindow("laba_02");

	// инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}

	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	// загрузка шейдера
	if (!shader.load(R"(assets\shaders\Example.vsh)", R"(assets\shaders\Example.fsh)"))
		return;

	QueryPerformanceCounter(&startCounter);
	QueryPerformanceFrequency(&frequency);
	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(reshape);
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(simulation);
	// основной цикл обработки сообщений ОС
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