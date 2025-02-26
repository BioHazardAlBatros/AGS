#include <windows.h>
#include "stdio.h"

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// используемые пространства имен (для удобства)
using namespace glm;
using namespace std;


// используемый шейдер (пока только один)
Shader shader;

// функция для вывода квадрата с ребрами равными единице (от -0.5 до +0.5)
void drawObject();

// функция вызывается при перерисовке окна
// в том числе и принудительно, по команде glutPostRedisplay
void display ()
{
	// отчищаем буфер кадра
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// включаем тест глубины (на всякий случай)
	glEnable(GL_DEPTH_TEST);

	// активируем шейдер
	shader.activate();

	// выводим прямоугольник 
	drawObject();

	// смена переднего и заднего буферов
	glutSwapBuffers();
}

// функция, вызываемая при изменении размеров окна
void reshape (int w,int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, w, h);
}

// функция вызывается когда процессор простаивает, т.е. максимально часто
void simulation()
{
	// перерисовать окно
	glutPostRedisplay();
};

// основная функция
void main(int argc, char **argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc,argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300,100);
	// устанавливаем размер окна
	glutInitWindowSize(800,600);
	// создание окна
	glutCreateWindow("laba_01");

	// инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
	  return;
	}

	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n",glGetString(GL_VERSION));

	// загрузка шейдера
	shader.load("SHADER\\Example.vsh", "SHADER\\Example.fsh");

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