#pragma once

#include "Data.h"
#include "Camera.h"
#include "Shader.h"
#include "GraphicObject.h"
#include "Material.h"
#include "Light.h"
#include "ResourceManager.h"
#include "FBO.h"

class RenderManager
{
	ResourceManager& rm;
	Camera* camera;
	std::vector<Shader> shaders;
	std::vector<FBO> fbos;
	std::vector<GraphicObject> graphicObjects;

	int windowHeight, windowWidth,postProcessingMode;

	RenderManager():rm(ResourceManager::instance()) {};
	RenderManager(const RenderManager& root) = delete;
	RenderManager& operator=(const RenderManager&) = delete;
public:
	int materialSwaps, textureSwaps,drawCalls;
	static RenderManager& instance()
	{
		static RenderManager renderManager;
		return renderManager;
	}

	bool init();
	void start();
	void setCamera(Camera* camera);
	void updateViewport(int w,int h);
	void addToRenderQueue(GraphicObject& graphicObject);
	void finish();
	void nextPostProcessing();
	void drawAllObjects();
	void drawQuad();
	int	 getPostProcessing();
};