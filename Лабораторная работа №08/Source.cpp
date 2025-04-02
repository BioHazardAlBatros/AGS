#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "GraphicObject.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Scene.h"

LARGE_INTEGER startCounter, frequency;
ResourceManager& rm = ResourceManager::instance();
RenderManager& renderMan = RenderManager::instance();

Camera camera;
Light sun;
Scene Demo;

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
	renderMan.start();
	Demo.draw();
	renderMan.finish();
	glutSwapBuffers();

//	static const char* postProcessMode[] = { "Normal","Grey","Sepia","Night","Dawn"};
	static const char* postProcessMode[] = { "Normal","Grey","Sepia"};
	char temp[128];
	sprintf_s(temp, "Lab8 FPS[%d] %s [%s mode] [%d matSwaps] [%d texSwaps] [%d drawcalls]", 
			getFPS(),Demo.getSceneDescription().c_str(),
			postProcessMode[renderMan.getPostProcessing()],
			renderMan.materialSwaps,renderMan.textureSwaps,renderMan.drawCalls);
	glutSetWindowTitle(temp);
}

void reshape(int w, int h)
{
	renderMan.updateViewport(w, h);
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

	//Change post-processing by pressing space
	if (GetAsyncKeyState(VK_SPACE)&1)
		renderMan.nextPostProcessing();

	float speedX = (GetAsyncKeyState(VK_DOWN) + (GetAsyncKeyState(VK_UP)) * (-1)) * 0.0005f * deltaTime, 
		  speedZ = (GetAsyncKeyState(VK_LEFT) + (GetAsyncKeyState(VK_RIGHT)) * (-1)) * 0.0002f * deltaTime;
	camera.moveOXZ(speedX,speedZ);
	
	glutPostRedisplay();
};

void initGraphicObjects();

void main(int argc, char** argv)
{
	ilInit();
	iluInit();
	ilutInit();
	glutInit(&argc, argv);
	if (!renderMan.init())
		return;
	renderMan.setCamera(&camera);
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
	Demo.init(R"(assets/models.json)");
	printf("All models are loaded. Loading scene...\n");
	Demo.loadFromJson(R"(assets/scenes/big_scene.json)");
	Demo.setCamera(&camera);
	Demo.setLight(&sun);
	printf("\nInitialization complete.\n");
}