#pragma once

#include "Data.h"
#include "Camera.h"
#include "Shader.h"
#include "GraphicObject.h"

class RenderManager
{
	RenderManager() {};
	RenderManager(const RenderManager& root) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

	Camera* camera;
	std::vector<Shader> shaders;
	std::vector<GraphicObject> graphicObjects;
public:
	static RenderManager& instance()
	{
		static RenderManager renderManager;
		return renderManager;
	}

	void init();
	void start();
	void setCamera(Camera* camera);
	void addToRenderQueue(GraphicObject& graphicObject);
	void finish();
};