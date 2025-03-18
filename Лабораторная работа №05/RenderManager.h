#pragma once

#include "Data.h"
#include "Camera.h"
#include "Shader.h"
#include "GraphicObject.h"
#include "ResourceManager.h"

class RenderManager
{
	ResourceManager& rm;
	Camera* camera;
	std::vector<Shader> shaders;
	std::vector<GraphicObject> graphicObjects;

	RenderManager():rm(ResourceManager::instance()) {};
	RenderManager(const RenderManager& root) = delete;
	RenderManager& operator=(const RenderManager&) = delete;
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