#pragma once
#include "Data.h"
#include "Camera.h"
#include "GraphicObject.h"
#include "Light.h"
#include "ResourceManager.h"
#include "RenderManager.h"

class Scene
{
	rapidjson::Document document;
	Camera* camera;
	Light* light;
	std::vector<GraphicObject> graphicObjects;
	int renderedObjectCount;

	GraphicObject createGraphicObject(std::string model);
	bool lodTest(GraphicObject& graphicObject);
	bool frustumCullingTest(GraphicObject& graphicObject);
public:
	Scene();
	void init(std::string sourceFile);
	bool loadFromJson(std::string sourceFile);
	void setCamera(Camera* camera);
	void setLight(Light* light);
	void draw();
	std::string getSceneDescription();
};

