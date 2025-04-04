#include "Shader.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "ResourceManager.h"


LARGE_INTEGER startCounter, frequency;
Camera camera;
Shader shader;
std::vector<GraphicObject> graphicObjects;
ResourceManager& rm = ResourceManager::instance();

double getSimulationTime()
{
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return temp.QuadPart - startCounter.QuadPart;
}

int getFPS()
{
	static int frameCount = 0;
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


void display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glm::mat4 viewMatrix = camera.getViewMatrix();

	shader.activate();
	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());

	for (auto& graphObj : graphicObjects) 
	{
		shader.setUniform("modelViewMatrix", viewMatrix * graphObj.getModelMatrix());
		shader.setUniform("color", graphObj.getColor());
		Mesh* mesh = rm.getMesh(graphObj.getMeshId());
		if (mesh) mesh->drawOne();
	}

	glutSwapBuffers();

	char temp[80];
	int FPS = getFPS();
	sprintf_s(temp, "Lab4 FPS[%d]", FPS);
	glutSetWindowTitle(temp);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	camera.setProjectionMatrix(35.0, (float)w / h, 1.0, 500.0);
}

void mouseZoom(int wheel, int dir, int x, int y)
{
	float delta = 0.5f*dir;
//	printf("Zooming %s\n",(dir==1)?"in":"out");
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

void initGraphicObjects();

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

	if (!shader.load(R"(assets\shaders\VertexShader.vsh)", R"(assets\shaders\FragmentShader.fsh)"))
		return; //MANUAL EXIT IF SHADERS DIDN'T COMPILE

	initGraphicObjects();
	
	QueryPerformanceCounter(&startCounter);
	QueryPerformanceFrequency(&frequency);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseWheelFunc(mouseZoom);
	glutIdleFunc(simulation);
	glutMainLoop();
	return;
}

void initGraphicObjects()
{
	ResourceManager& rm = ResourceManager::instance();
	GraphicObject gObj;

	//1
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\buildings\house_2.obj)"));
	gObj.setColor(glm::vec4{ 0.2, 0.2, 0.2, 1 });
	gObj.setPosition(glm::vec3{ 0, 0, 0 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//2
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\natures\big_tree.obj)"));
	gObj.setColor(glm::vec4{ 0.2, 0.8, 0.2, 1 });
	gObj.setPosition(glm::vec3{ 7.5, -0.75, 2.5 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//3
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\natures\big_tree.obj)"));
	gObj.setColor(glm::vec4{ 0.2, 0.8, 0.2, 1 });
	gObj.setPosition(glm::vec3{ -7.5, -0.75, 2.5 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//4
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\police_car.obj)"));
	gObj.setColor(glm::vec4{ 0.2, 0.2, 1.0, 1 });
	gObj.setPosition(glm::vec3{ +4.5, -2.15, +6.5 });
	gObj.setAngle(-115.0);
	graphicObjects.push_back(gObj);

	//5
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\police_car.obj)"));
	gObj.setColor(glm::vec4{0.23, 0.23, 1.0, 1});
	gObj.setPosition(glm::vec3{ +4.25, -2.15, +10.5 });
	gObj.setAngle(+105.0);
	graphicObjects.push_back(gObj);

	//6
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\jeep.obj)"));
	gObj.setColor(glm::vec4{ 0.95, 0.13, 0.13, 1 });
	gObj.setPosition(glm::vec3{ -1.25, -2.15, +9.0 });
	gObj.setAngle(+170.0);
	graphicObjects.push_back(gObj);
	
	printf("\nInitialization complete.");
}