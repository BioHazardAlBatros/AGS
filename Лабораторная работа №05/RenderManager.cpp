#include "RenderManager.h"

void RenderManager::init() 
{
	Shader shader;
	if (!shader.load(R"(assets\shaders\VertexShader.vsh)", R"(assets\shaders\FragmentShader.fsh)"))
		return;
	this->shaders.push_back(shader);
}
void RenderManager::start() 
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	this->shaders[0].activate();
	this->shaders[0].setUniform("projectionMatrix", this->camera->getProjectionMatrix());
	this->shaders[0].setUniform("texture_0", 0);

	glActiveTexture(GL_TEXTURE0);
}
void RenderManager::setCamera(Camera* camera) 
{
	this->camera = camera;
}
void RenderManager::addToRenderQueue(GraphicObject& graphicObject) 
{
	this->graphicObjects.push_back(graphicObject);
}
void RenderManager::finish() 
{
	glm::mat4 viewMatrix = this->camera->getViewMatrix();
	for (auto& graphObj : this->graphicObjects)
	{
		this->shaders[0].setUniform("modelViewMatrix", viewMatrix * graphObj.getModelMatrix());
		this->shaders[0].setUniform("color", graphObj.getColor());

		Texture* texture = rm.getTexture(graphObj.getTextureId());
		if (texture) texture->bind();

		Mesh* mesh = rm.getMesh(graphObj.getMeshId());
		if (mesh) mesh->drawOne();
	}
	this->graphicObjects = std::vector<GraphicObject>();
}