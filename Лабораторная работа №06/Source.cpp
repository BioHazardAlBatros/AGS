#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "GraphicObject.h"
#include "ResourceManager.h"
#include "RenderManager.h"

LARGE_INTEGER startCounter, frequency;
Camera camera;
Light sun;
std::vector<GraphicObject> graphicObjects;
ResourceManager& rm = ResourceManager::instance();
RenderManager& renderMan = RenderManager::instance();

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
	for (auto& graphObj : graphicObjects)
		renderMan.addToRenderQueue(graphObj);
	renderMan.finish();
	
	glutSwapBuffers();

	char temp[80];
	int FPS = getFPS();
	sprintf_s(temp, "Lab5 FPS[%d]", FPS);
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
	ilInit();
	iluInit();
	ilutInit();
	glutInit(&argc, argv);
	renderMan.init();
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
	ResourceManager& rm = ResourceManager::instance();
	GraphicObject gObj;

	//1
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\buildings\house_2.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\dull_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\buildings\house_2_orange.png)"));
	gObj.setColor(glm::vec4{ 0.2, 0.2, 0.2, 1 });
	gObj.setPosition(glm::vec3{ 0, 0, 0 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//2
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\natures\big_tree.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\shiny_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\natures\nature.png)"));
	gObj.setColor(glm::vec4{ 0.2, 0.8, 0.2, 1 });
	gObj.setPosition(glm::vec3{ 7.5, -0.75, 2.5 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//3
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\natures\big_tree.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\dull_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\natures\nature.png)"));
	gObj.setColor(glm::vec4{ 0.2, 0.8, 0.2, 1 });
	gObj.setPosition(glm::vec3{ -7.5, -0.75, 2.5 });
	gObj.setAngle(0.0);
	graphicObjects.push_back(gObj);

	//4
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\police_car.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\shiny_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\vehicles\police_car.png)"));
	gObj.setColor(glm::vec4{ 0.2, 0.2, 1.0, 1 });
	gObj.setPosition(glm::vec3{ +4.5, -2.15, +6.5 });
	gObj.setAngle(-115.0);
	graphicObjects.push_back(gObj);

	//5
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\police_car.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\shiny_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\vehicles\police_car.png)"));
	gObj.setColor(glm::vec4{0.23, 0.23, 1.0, 1});
	gObj.setPosition(glm::vec3{ +4.25, -2.15, +10.5 });
	gObj.setAngle(+105.0);
	graphicObjects.push_back(gObj);

	//6
	gObj.setMeshId(rm.loadMesh(R"(assets\meshes\vehicles\jeep.obj)"));
	gObj.setMaterialId(rm.loadMaterial(R"(assets\materials\shiny_material.json)"));
	gObj.setTextureId(rm.loadTexture(R"(assets\textures\vehicles\jeep_purple.png)"));
	gObj.setColor(glm::vec4{ 0.95, 0.13, 0.13, 1 });
	gObj.setPosition(glm::vec3{ -1.25, -2.15, +9.0 });
	gObj.setAngle(+170.0);
	graphicObjects.push_back(gObj);
	
	printf("\nInitialization complete.");
}